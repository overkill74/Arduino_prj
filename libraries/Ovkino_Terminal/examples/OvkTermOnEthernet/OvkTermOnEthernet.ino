//-----------------------------------------------------------------------
// Author: Ivan Zoli 2022
//-----------------------------------------------------------------------
// Terminal example
//-----------------------------------------------------------------------

#include <terminale.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 12, 100);
IPAddress myDns(192, 168, 12, 1);
IPAddress gateway(192, 168, 12, 1);
IPAddress subnet(255, 255, 0, 0);
const uint16_t g_server_port = 3333;

// telnet defaults to port g_server_port
EthernetServer g_server(g_server_port);
bool alreadyConnected = false; // whether or not the client was connected previously



//-----------------------------------------------------------------------
static int g_param=0; //Parameter

//-----------------------------------------------------------------------
static int prm_inc(int argc, char** argv, Stream& str)
{
  int delta = 1;
  if (argc > 1) {
    delta = atoi(argv[1]);
  }

  g_param += delta;
  str.print("g_param: ");
  str.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
static int prm_dec(int argc, char** argv, Stream& str)
{
  int delta = 1;
  if (argc > 1) {
    delta = atoi(argv[1]);
  }

  g_param -= delta;
  str.print("g_param: ");
  str.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
static int prm_val(int argc, char** argv, Stream& str)
{
  int delta = 1;
  if (argc > 1) {
    g_param = atoi(argv[1]);
  }

  str.print("g_param: ");
  str.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
static const TermCmd g_term_cmd[] =
{
  // <PAR obbligatorio>
  // [PAR opzionale]

  //-------------------------------------------------------------------------
  { "INC",  prm_inc ,  0, "[delta] inc of delta to parameter"        },
  { "DEC",  prm_dec ,  0, "[delta] dec of delta to parameter"        },
  { "VAL",  prm_val ,  0, "[new_val] get or set parameter  "         },
  TERMINALE_FINE_LISTA
};

//-----------------------------------------------------------------------
static Terminale g_term(g_term_cmd, Serial);
static Terminale* g_term_on_eth = nullptr;

//-----------------------------------------------------------------------
void setup()
{
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // initialize the Ethernet device
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start listening for clients
  g_server.begin();

  Serial.print("Chat server address -> ");
  Serial.print(Ethernet.localIP());
  Serial.print(" : ");
  Serial.println(g_server_port);
  
}

  
//-----------------------------------------------------------------------
void loop()
{
  // wait for a new client:
  EthernetClient client = g_server.available();
  if (client) {
    if (!g_term_on_eth) {
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      client.flush();
      g_term_on_eth = new Terminale(g_term_cmd, client);
    }
    g_term_on_eth->doWorkAndAnswer();
  }
  else {
    delete g_term_on_eth;
    g_term_on_eth = nullptr;
  }

  g_term.doWorkAndAnswer();
}
