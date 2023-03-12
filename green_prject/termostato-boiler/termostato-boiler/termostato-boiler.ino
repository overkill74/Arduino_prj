#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Dichiarazioni delle costanti per la connessione WiFi
const char* ssid = "FASTWEB-9n9CGH";
const char* password = "Su2r9s59Zy";

// Dichiarazioni delle costanti per il server web
ESP8266WebServer server(80);

// Dichiarazioni delle costanti per il LED e il bottone
#define LED_PIN D7 // GPIO 5
#define BUTTON_PIN D2 // GPIO 4

// Variabile per lo stato del LED
bool ledState = false;

#if 0

// Funzione per la gestione della richiesta HTTP GET
void handleRoot() {
  String html = "<html><body><h1>Server HTTP per accendere/spegnere un LED</h1>";
  html += "<p>LED: ";
  if (ledState) {
    html += "Acceso</p>";
  }
  else {
    html += "Spento</p>";
  }
  html += "<button onclick=\"location.href='/toggle'\">Accendi/Spegni</button></body></html>";
  server.send(200, "text/html", html);
}

// Funzione per accendere/spegnere il LED
void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  Serial.println("Stato LED: " + String(ledState));
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  // Inizializzazione del LED e del bottone
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione in corso...");
  }
  // Stampa l'indirizzo IP sulla seriale
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Impostazione dei gestori per le richieste HTTP
  server.on("/", handleRoot);
  server.on("/toggle", toggleLED);

  // Avvio del server HTTP
  server.begin();
}

void loop() {
  // Gestione delle richieste HTTP
  server.handleClient();

  // Controllo dello stato del bottone
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      toggleLED();
    }
  }
}

#endif

// Variabile per la luminosità del LED
int brightness = 128;

void handleRoot() {
  String html2 = "<!DOCTYPE html>\
<html>\
  <head>\
    <meta charset=\"UTF-8\">\
    <title>Slider LED</title>\
  </head>\
  <body>\
    <h1>Slider LED</h1>\
    <input type=\"range\" min=\"0\" max=\"255\" value=\"0\" id=\"slider\">\
    <div id=\"status\"></div>\
    <script>\
      var slider = document.getElementById(\"slider\");\
      var status = document.getElementById(\"status\");\
      slider.onchange = function() {\
        var value = slider.value;\
        var xhr = new XMLHttpRequest();\
        xhr.open(\"POST\", \"/led\", true);\
        xhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");\
        xhr.onreadystatechange = function() {\
          if (xhr.readyState === 4 && xhr.status === 200) {\
            var response = xhr.responseText;\
            status.innerHTML = response;\
          }\
        };\
        xhr.send(\"brightness=\" + value);\
      };\
    </script>\
  </body>\
</html>";
  String html = "<html><body><h1>Server HTTP per controllare la luminosità di un LED</h1>";
  html += "<p>Luminosità: " + String(brightness) + "</p>";
  html += "<form method='POST' action='/set_brightness'>";
  html += "<input type='range' min='0' max='1023' value='" + String(brightness) + "' onchange='this.form.submit()'>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html2);
}

void setBrightness() {
  brightness = server.arg("brightness").toInt();
  Serial.print("Luminosità: ");
  Serial.println(brightness);
  analogWrite(LED_PIN, brightness);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void setup() {
  // Inizializzazione del LED
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  Serial.begin(115200);

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione in corso...");
  }

  // Stampa l'indirizzo IP sulla seriale
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Impostazione dei gestori per le richieste HTTP
  server.on("/", handleRoot);
  server.on("/set_brightness", setBrightness);

  // Avvio del server HTTP
  server.begin();
}

void loop() {
  // Gestione delle richieste HTTP
  server.handleClient();
}
