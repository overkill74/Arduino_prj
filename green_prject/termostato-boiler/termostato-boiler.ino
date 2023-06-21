#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "twat.h"
#include "web-time.h"

// Dichiarazioni delle costanti per il LED e il bottone
#define SENSOR_GND  D8 // - Sensore GND
#define SENSOR_PWR  D7 // - Sensore +Vcc
#define SENSOR_DATA D6 // - Sensore Data
#define LED_PIN     D3 // - Led
#define BUTTON_PIN  D2 // - Button

// Dichiarazioni delle costanti per la connessione WiFi
const char* ssid = "FASTWEB-9n9CGH";
const char* password = "Su2r9s59Zy";

// Dichiarazioni delle costanti per il server web
ESP8266WebServer server(80);
ESP8266WebServer server_ext(1080);
TWater g_twat(SENSOR_DATA);
WebTime g_web_time;

// Variabile per lo stato del LED
bool g_output = false;
bool g_power = false;
float g_water_temp = 20;      //! Temperatura dell'acqua
float g_set_point = 35;       //! Set point temperatura
const float g_max_temp = 55;  //! Massimo setpoint
const float g_min_temp = 5;   //! Minimo setpoint

//---------------------------------------------------------
/// \brief ricarica la pagina
static void reload_page()
{
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");

  server_ext.sendHeader("Location", "/");
  server_ext.send(302, "text/plain", "");
}

/// \brief Incrementa il setpoint
void setpointInc()
{
  g_set_point += 1;
  if (g_set_point > g_max_temp) {
    g_set_point = g_max_temp;
  }
  reload_page();
}

/// \brief Decrementa il setpoint
void setpointDec()
{
  g_set_point -= 1;
  if (g_set_point < g_min_temp) {
    g_set_point = g_min_temp;
  }
  reload_page();
}

/// \brief Funzione per accendere/spegnere il LED
void toggleLED() {
  g_output = !g_output;
  digitalWrite(LED_PIN, g_output);
  Serial.println("Stato LED: " + String(g_output));
  reload_page();
}

// Funzione per la gestione della richiesta HTTP GET
void handleRoot()
{
  String html = "<html>"
  "<head>"
  "<title>Boiler del bagno</title>"
  "<meta http-equiv=\"refresh\" content=\"5\">"
  "</head>"
  "<body>"
  "<h1>Boiler del bagno</h1>";

  html += "<h4>Ora: " + g_web_time.getDateTime() + "</h4>";

  // Temperatura
  char txt[64];
  dtostrf(g_water_temp, 4, 1, txt);
  html += "<h2>T acqua  : " + String(txt) + "&deg;C</h2>";

  // Tasti set point
  dtostrf(g_set_point, 4, 1, txt);
  html += "<h3>Set Point: " + String(txt) + "&deg;C  ";
  html += "<button onclick=\"location.href='/setpoint_dec'\">- Setpoint</button>";
  html += "<button onclick=\"location.href='/setpoint_inc'\">+ Setpoint</button></h3>";

  // Led
  html += "<hr><p>LED: ";
  html += g_output ? "Acceso</p>" : "Spento</p>";
  html += "<button onclick=\"location.href='/toggle'\">Accendi/Spegni</button></body></html>";

  server.send(200, "text/html", html);
  server_ext.send(200, "text/html", html);
}


static bool handle_connection()
{
    // Gestione connessione
  static bool connected = false;

  if (WiFi.status() != WL_CONNECTED) {
    connected = false;
    Serial.println("Connessione in corso...");
  }
  else {
    if (!connected) {
      // Stampa l'indirizzo IP sulla seriale
      Serial.print("Indirizzo IP: ");
      Serial.println(WiFi.localIP());
    }
    connected = true;
  }

  return connected;
}

static void ctrl_temperature()
{
  // Legge la temperatura
  float temp = g_twat.readTemperature();
  if (temp <= T_ERRROR) {

  }
  g_water_temp = temp;
}

void setup()
{
  // Inizializzazione del sensore
  pinMode(SENSOR_GND, OUTPUT);
  digitalWrite(SENSOR_GND, LOW);

  pinMode(SENSOR_PWR, OUTPUT);
  digitalWrite(SENSOR_PWR, HIGH);

  // Inizializzazione del LED e del bottone
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  // Impostazione dei gestori per le richieste HTTP
  server.on("/", handleRoot);
  server.on("/toggle", toggleLED);
  server.on("/setpoint_inc", setpointInc);
  server.on("/setpoint_dec", setpointDec);

  server_ext.on("/", handleRoot);
  server_ext.on("/toggle", toggleLED);
  server_ext.on("/setpoint_inc", setpointInc);
  server_ext.on("/setpoint_dec", setpointDec);

  // Avvio del server HTTP
  server.begin();
  server_ext.begin();

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
}

void loop()
{
  static uint32_t tout = 0;

  if (millis() > tout)
  {
    tout = millis() + 1000;

    handle_connection();

    ctrl_temperature();
  }

  // Gestione delle richieste HTTP
  server.handleClient();
  server_ext.handleClient();

  // Controllo dello stato del bottone
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      toggleLED();
    }
  }
}

