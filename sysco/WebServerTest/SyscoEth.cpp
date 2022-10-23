//--------------------------------------------------------------------------------
#include "SyscoEth.h"

//--------------------------------------------------------------------------------
SyscoEth::SyscoEth()
{

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SyscoEth* SyscoEth::m_instance = nullptr;
//--------------------------------------------------------------------------------
SyscoEth* SyscoEth::createInstance()
{
  if (!m_instance) {
    m_instance = new SyscoEth();
  }
  return m_instance;
}
//--------------------------------------------------------------------------------
SyscoEth* SyscoEth::getInstance()
{
  return m_instance;
}


#if 0

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 12, 100);


// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet


void setup_foo()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);  // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  g_display->drawIp(static_cast<uint32_t>(Ethernet.localIP()));
}


static void ethernet_maintain()
{
  switch (Ethernet.maintain()) {
    case 1:
      Serial.println("Error: renewed fail");
      break;
    case 2:
      Serial.println("Renewed success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      g_display->drawIp(static_cast<uint32_t>(Ethernet.localIP()));
      break;
    case 3:
      Serial.println("Error: rebind fail");
      break;
    case 4:
      Serial.println("Rebind success");
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      g_display->drawIp(static_cast<uint32_t>(Ethernet.localIP()));
      break;
    default:
      Serial.print('.');
      break;
  }
}


void foo_loop()
{
  ethernet_maintain();
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          String to_send;
          to_send.concat("HTTP/1.1 200 OK\n");
          to_send.concat("Content-Type: text/html\n");
          to_send.concat("Connection: close\n");  // the connection will be closed after completion of the response
          to_send.concat("Refresh: 1\n");         // refresh the page automatically every 5 sec
          to_send.concat("\n");
          to_send.concat("<!DOCTYPE HTML>");
          to_send.concat("<html>");
          to_send.concat("<style>table, th, td {border:1px solid black;}</style>");
          to_send.concat("<table style=\"width:50%\">");

          // Header
          to_send.concat("<tr>");
          to_send.concat("<th>Input #</th>");
          to_send.concat("<th>ADC</th>");
          to_send.concat("<th>mV</th>");
          to_send.concat("<th>&deg;C</th>");
          to_send.concat("</tr>");

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            to_send.concat("<tr>");

            to_send.concat("<td>");
            to_send.concat(analogChannel);
            to_send.concat("</td>");

            int sensorReading = analogRead(analogChannel);
            to_send.concat("<td>");
            to_send.concat(sensorReading);
            to_send.concat("</td>");

            float milliv = (float)sensorReading * (5000.0f / 1024.0f);
            to_send.concat("<td>");
            to_send.concat(milliv);
            to_send.concat("</td>");

            float celsius = (float)milliv / 10.0f;
            to_send.concat("<td>");
            to_send.concat(celsius);
            to_send.concat("</td>");

            to_send.concat("</tr>");
          }
          to_send.concat("</table>");
          to_send.concat("</html>");

          client.println(to_send);


          //--------------------------------------------------------
          /*
          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.print("<!DOCTYPE HTML>");
          client.print("<html>");
          client.print("<style>table, th, td {border:1px solid black;}</style>");

          client.print("<table style=\"width:50%\">");

          // Header
          client.print("<tr>");
          client.print("<th>Input #</th>");
          client.print("<th>ADC</th>");
          client.print("<th>mV</th>");
          client.print("<th>&deg;C</th>");
          client.print("</tr>");

          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++)
          {
            client.print("<tr>");

            client.print("<td>");
            client.print(analogChannel);
            client.print("</td>");
            
            int sensorReading = analogRead(analogChannel);
            client.print("<td>");
            client.print(sensorReading);
            client.print("</td>");

            float milliv = (float)sensorReading * (5000.0f / 1024.0f);
            client.print("<td>");
            client.print(milliv);
            client.print("</td>");

            float celsius = (float)milliv / 10.0f;
            client.print("<td>");
            client.print(celsius);
            client.print("</td>");

            client.print("</tr>");
          }
          client.print("</table>");
          client.println("</html>");
          */
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

#endif