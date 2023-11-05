#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "twat.h"
#include "web-time.h"
#include_next <Ticker.h>
#include <EEPROM.h>



class StoredData
{
private:
  class StoreVariables {
    public:
      float     m_set_point = 35;         //! Set point temperatura
      float     m_set_point_doccia = 45;  //! Set point temperatura doccia
      uint32_t  m_energy = 0;             //! Energy
      union Flags {
        struct {
          uint32_t    is_dst  : 1;        //! Is Daylight saving time
        } b;
        uint32_t all;
      } flags;
  };

public:
  StoredData() {
  }

  void begin() {
    EEPROM.begin(512);
    read_data();
  }

  ///
  /// \brief 
  float incSetPoint() {
    if (m_stored_vars.m_set_point >= m_max_temp) {
      m_stored_vars.m_set_point = m_max_temp;
    }
    else {
      m_stored_vars.m_set_point += 1;
    }
    store_data();
    return m_stored_vars.m_set_point;
  }
  ///
  /// \brief 
  float incSetPointDoccia() {
    if (m_stored_vars.m_set_point_doccia >= m_max_temp) {
      m_stored_vars.m_set_point_doccia = m_max_temp;
    }
    else {
      m_stored_vars.m_set_point_doccia += 1;
    }
    store_data();
    return m_stored_vars.m_set_point_doccia;
  }
  ///
  /// \brief 
  uint32_t incEnergy(uint32_t delta) {
    m_stored_vars.m_energy += delta / 100;
    if (++m_cnt_store_energy >= 5*60) {
      store_data();
    }
    // Serial.print("Energy : ");
    // Serial.println(m_energy);
    // Serial.print("m_cnt_store_energy : ");
    // Serial.println(m_cnt_store_energy);
    return m_stored_vars.m_energy;
  }
  ///
  /// \brief 
  void saveEnergy() {
    store_data();
  }
  ///
  /// \brief 
  void resetEnergy() {
    m_stored_vars.m_energy = 0;
    store_data();
  }

  ///
  /// \brief 
  float decSetPoint() {
    if (m_stored_vars.m_set_point <= m_min_temp) {
      m_stored_vars.m_set_point = m_min_temp;
    }
    else {
      m_stored_vars.m_set_point -= 1;
    }
    store_data();
    return m_stored_vars.m_set_point;
  }
  ///
  /// \brief 
  float decSetPointDoccia() {
    if (m_stored_vars.m_set_point_doccia <= m_min_temp) {
      m_stored_vars.m_set_point_doccia = m_min_temp;
    }
    else {
      m_stored_vars.m_set_point_doccia -= 1;
    }
    store_data();
    return m_stored_vars.m_set_point_doccia;
  }
  ///
  /// \brief 
  bool toggleIsDst() {
    m_stored_vars.flags.b.is_dst ^= 1;
    return m_stored_vars.flags.b.is_dst;
  }
  ///
  /// \brief 
  float getSetPoint() {
    return m_stored_vars.m_set_point;
  }
  ///
  /// \brief 
  float getSetPointDoccia() {
    return m_stored_vars.m_set_point_doccia;
  }
  ///
  /// \brief 
  float getEnergy() {
    return (float)(((m_stored_vars.m_energy + 500) / 10) / 3600.0f) * m_r_pow;
  }
  ///
  /// \brief 
  bool isDst() {
    return m_stored_vars.flags.b.is_dst;
  }

private:
  void store_data() {
    int ee = 0;
    byte* p = (byte*)(void*)&m_stored_vars;
    for (int i = 0; i < sizeof(m_stored_vars); i++) {
      EEPROM.write(ee++, *p++);
    }
    EEPROM.commit();  // Salva effettivamente i dati
    m_cnt_store_energy = 0;
  }

  void read_data() {
    int ee = 0;
    byte* p = (byte*)(void*)&m_stored_vars;
    for (int i = 0; i < sizeof(m_stored_vars); i++) {
      *p++ = EEPROM.read(ee++);
    }
    if (isnan(m_stored_vars.m_set_point)) m_stored_vars.m_set_point = 35;
    if (isnan(m_stored_vars.m_set_point_doccia)) m_stored_vars.m_set_point_doccia = 45;
    if (m_stored_vars.m_energy == 0xFFFFFFFF) m_stored_vars.m_energy = 0;
    
    if (m_stored_vars.m_set_point < m_min_temp) m_stored_vars.m_set_point = 35;
    if (m_stored_vars.m_set_point > m_max_temp) m_stored_vars.m_set_point = 35;
    if (m_stored_vars.m_set_point_doccia < m_min_temp) m_stored_vars.m_set_point_doccia = 45;
    if (m_stored_vars.m_set_point_doccia > m_max_temp) m_stored_vars.m_set_point_doccia = 45;
/*
    Serial.print("m_set_point        : "); Serial.println(m_set_point); 
    Serial.print("m_set_point_doccia : "); Serial.println(m_set_point_doccia); 
    Serial.print("m_max_temp         : "); Serial.println(m_max_temp); 
    Serial.print("m_min_temp         : "); Serial.println(m_min_temp); 
*/
  }

private:
  StoreVariables  m_stored_vars;  //! Store variables

  int m_cnt_store_energy = 0;     //! Counter to store updated energy
  const float m_max_temp = 55;    //! Massimo setpoint
  const float m_min_temp = 5;     //! Minimo setpoint
  const float m_r_pow = 1200;     //! Power consumed by resistance
};


// Dichiarazioni delle costanti per il LED e il bottone
#define SENSOR_DATA D4 // - Sensore Data
#define LED_PIN     D6 // - Led
#define RELE_PIN    D5 // - RELE

// Dichiarazioni delle costanti per la connessione WiFi
const char* ssid = "FASTWEB-9n9CGH";
const char* password = "Su2r9s59Zy";

// Dichiarazioni delle costanti per il server webu
ESP8266WebServer server(80);
ESP8266WebServer server_ext(1080);
TWater g_twat(SENSOR_DATA);
WebTime g_web_time;
Ticker g_led_ticker;
StoredData g_store_data;

// Variabile per lo stato del LED
bool g_power = false;

bool g_connected = false;
bool g_temp_ok = false;
bool g_out_rele = false;

float g_water_temp = 20;          //! Temperatura dell'acqua

bool g_is_doccia_scheduled = false;
uint32_t g_tout_ch_doccia = 0;

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
  g_store_data.incSetPoint();
  reload_page();
}

/// \brief Decrementa il setpoint
void setpointDec()
{
  g_store_data.decSetPoint();
  reload_page();
}

/// \brief Incrementa il setpoint doccia
void setpointDocciaInc()
{
  g_store_data.incSetPointDoccia();
  reload_page();
}

/// \brief Decrementa il setpoint doccia
void setpointDocciaDec()
{
  g_store_data.decSetPointDoccia();
  reload_page();
}

/// \brief Prenota la doccia
void prenotaDocciaSet()
{
  g_is_doccia_scheduled = true;
  g_tout_ch_doccia = millis() + 2 * 3600 * 1000;
  reload_page();
}

/// \brief Cancella la prenotazione
void prenotaDocciaClr()
{
  g_is_doccia_scheduled = false;
  g_tout_ch_doccia = 0;
  reload_page();
}

/// \brief Cancella la prenotazione
void resetEnergy()
{
  g_store_data.resetEnergy();
  reload_page();
}

/// \brief Toggle DST
void toggleDst()
{
  g_web_time.setDst(g_store_data.toggleIsDst());
  reload_page();
}

// Funzione per la gestione della richiesta HTTP GET
void handleRoot()
{
  String html = "<html>"
  "<head>"
  "<title>Boiler del bagno</title>"
  "<meta http-equiv=\"refresh\" content=\"5\">"
  "<style>"
  "  table {"
  "    border-collapse: separate;"
  "    border-spacing: 10px;"
  //"    background-color: lightgray;"
  "    text-align: left;"
  //"    border: 1px solid black;"
  "    font-size: 25px;"
  "  }"
  "  tr {"
  "    height: 30px;"
  "  }"
  "  td {"
  "    padding: 0px 0px 0px 30px ;"
  "  }"
  "  td:first-child {"
  "    padding-left: 0;"
  "  }"
  "</style>"
  "</head>"
  "<body>"
  "<h1>Boiler del bagno</h1>";

  html += "<table><tr>";
  html += "<td><b>Ora: " + g_web_time.getDateTime() + "</b></td>";
  if (g_store_data.isDst()) {
    html += "<td><button onclick=\"location.href='/toggle_dst'\">Clear DST</button></td>";
  }
  else {
    html += "<td><button onclick=\"location.href='/toggle_dst'\">Set DST</button></td>";
  }
  html += "</tr></table>";

  // Temperatura
  char txt[64];
  dtostrf(g_water_temp, 4, 1, txt);
  html += "<h2>T acqua  : " + String(txt) + "&deg;C</h2>";

  // Tasti set point
  dtostrf(g_store_data.getSetPoint(), 4, 1, txt);
  if (!g_is_doccia_scheduled) {
    html += "<h3><font color=\"green\">&rarr;";
  }
  else {
    html += "<h3><font color=\"black\">";
  }
  html += "Set Point : " + String(txt) + "&deg;C  </font>";
  html += "<button onclick=\"location.href='/setpoint_dec'\">- 1</button>";
  html += "<button onclick=\"location.href='/setpoint_inc'\">+ 1</button>";

  // Tasti set point Doccia
  dtostrf(g_store_data.getSetPointDoccia(), 4, 1, txt);
  if (g_is_doccia_scheduled) {
    html += "<h3><font color=\"green\">&rarr;";
  }
  else {
    html += "<h3><font color=\"black\">";
  }
  html += "Set Point Doccia : " + String(txt) + "&deg;C  </font>";
  html += "<button onclick=\"location.href='/sp_doccia_dec'\">- 1</button>";
  html += "<button onclick=\"location.href='/sp_doccia_inc'\">+ 1</button></h3>";

  // Tasti set prenotare la doccia
  if (g_is_doccia_scheduled) {
    html += "<hr><h3><font color=\"red\">Doccia Prenotata   </font>";
    html += "<button onclick=\"location.href='/sch_doccia_clr'\">Annulla</button></h3>";
  }
  else {
    html += "<hr><h3><font color=\"blue\">Doccia non prenotata   </font>";
    html += "<button onclick=\"location.href='/sch_doccia_set'\">PRENOTA</button></h3>";
  }

  // Led
  html += "<hr><p>OUTPUT: ";
  html += g_out_rele ? "<mark>Acceso</mark></p>" : "Spento</p><hr>";

  // Power consumption
  dtostrf((float)g_store_data.getEnergy() / 1000.0f , 4, 2, txt);
  html += "<h3>Energia consumata : " + String(txt) + "kWh    ";
  html += "<button onclick=\"location.href='/reset_energy'\">RESET</button>";
  
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

static bool ctrl_temperature()
{
  static uint32_t t_out = 0;
  
  if (g_is_doccia_scheduled) {
    if (millis() > g_tout_ch_doccia) {
      g_tout_ch_doccia = 0;
      g_is_doccia_scheduled = false;
    }
  }
  float sp = g_is_doccia_scheduled ? g_store_data.getSetPointDoccia() : g_store_data.getSetPoint();
  
  // Legge la temperatura
  float temp = g_twat.readTemperature();
  if (temp <= T_ERRROR) {
    return false;
  }
  g_water_temp = temp;

  if (g_out_rele) {
    uint32_t now = millis();
    g_store_data.incEnergy(now - t_out);
    t_out = now;
    // Uscita attiva
    if (g_water_temp >= (sp + 0.5)) {
      g_out_rele = false;
      g_store_data.saveEnergy();
    }
  }
  else {
    // Uscita spenta
    if (g_water_temp <= (sp - 2)) {
      g_out_rele = true;
      t_out = millis();
    }
  }
  
  digitalWrite(RELE_PIN, g_out_rele);

  return true;
}

static void ctrl_led()
{
  static int32_t tout_led = 0;
  static bool led_phase = false;
  static int missing_cnt_tout = 10;

  if (--tout_led <= 0)
  {
    if (!g_temp_ok) {
      // Lampeggio Errore
      led_phase ^= true;
      tout_led = 10;
    }
    else {
      if (!g_connected) {
        // Lampeggio mancata connessione
        led_phase ^= true;
        tout_led = 50;
      }
      else {
        led_phase = g_out_rele;
      }
    }
  }

  digitalWrite(LED_PIN, led_phase);
}

void setup()
{
  Serial.begin(115200);

  // Setup
  g_store_data.begin();

  // Inizializzazione del LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  g_led_ticker.attach_ms(10, ctrl_led);

  // Inizializzazione uscita rele
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);

  // Impostazione dei gestori per le richieste HTTP
  server.on("/", handleRoot);
  server.on("/setpoint_inc", setpointInc);
  server.on("/setpoint_dec", setpointDec);
  server.on("/sp_doccia_inc", setpointDocciaInc);
  server.on("/sp_doccia_dec", setpointDocciaDec);
  server.on("/sch_doccia_set", prenotaDocciaSet);
  server.on("/sch_doccia_clr", prenotaDocciaClr);
  server.on("/reset_energy", resetEnergy);
  server.on("/toggle_dst", toggleDst);

  server_ext.on("/", handleRoot);
  server_ext.on("/setpoint_inc", setpointInc);
  server_ext.on("/setpoint_dec", setpointDec);
  server_ext.on("/sp_doccia_inc", setpointDocciaInc);
  server_ext.on("/sp_doccia_dec", setpointDocciaDec);
  server_ext.on("/sch_doccia_set", prenotaDocciaSet);
  server_ext.on("/sch_doccia_clr", prenotaDocciaClr);
  server_ext.on("/reset_energy", resetEnergy);
  server_ext.on("/toggle_dst", toggleDst);

  // Avvio del server HTTP
  server.begin();
  server_ext.begin();

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  
  // Imposta DST
  g_web_time.setDst(g_store_data.isDst());
}

void loop()
{
  static uint32_t tout = 0;

  if (millis() > tout)
  {
    tout = millis() + 1000;

    g_connected = handle_connection();

    g_temp_ok = ctrl_temperature();
  }

  // Gestione delle richieste HTTP
  server.handleClient();
  server_ext.handleClient();

  // Controllo dello stato del bottone
/*
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      toggleLED();
    }
  }
  */
}

