////////////////////////////////////////////////////////////////////////////////
//                           I N C L U D E S                                  //
////////////////////////////////////////////////////////////////////////////////

// System Library Include
#include <Arduino.h>
#include <HardwareSerial.h>


// User Library Include
#include "termargs.h"
#include "terminale.h"

////////////////////////////////////////////////////////////////////////////////
//              P R I V A T E   S T A T I C   F U N C T I O N S               //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                        //
////////////////////////////////////////////////////////////////////////////////
Terminale::Terminale(const TermCmd* comandi, size_t buff_sz)
  : m_commands(comandi)
  , m_rx_buff_sz(buff_sz)
{
  if (m_rx_buff_sz < 16) {
    m_rx_buff_sz = 16;
  }
  m_rx_buffer = new char[m_rx_buff_sz];
  if (!m_rx_buffer) {
    Serial.println("No buffer for terminal");
    m_rx_buff_sz = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Terminale::clear_rx()
{
  while (Serial.available()) {
    Serial.read();
  }
  m_cnt = 0;
  m_overflow = false;
  m_rx_complete = false;
}

////////////////////////////////////////////////////////////////////////////////
bool Terminale::rx_command()
{
  while (Serial.available()) {
    char rxc = Serial.read();
    if (m_cnt < (m_rx_buff_sz-1)) {
      if (rxc >= ' ') {
	m_rx_buffer[m_cnt++] = rxc;
      }
    }
    else {
      m_overflow = true;
    }
    if (rxc == '\n' || rxc == '\r') {
      m_rx_complete = true;
      m_rx_buffer[m_cnt++] = '\0';
    }
  }
  return m_rx_complete;
}

////////////////////////////////////////////////////////////////////////////////
char* const Terminale::get_command()
{
  if (m_rx_complete) {
    return m_rx_buffer;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void Terminale::doWorkAndAnswer()
{
  if (!rx_command()) {
    // Not Rx complete
    return;
  }

  // Parse string
  char* argv[8];
  int argc = argSegment(m_rx_buffer, argv, 8);

  // Capitalize command
  char* cmd = argv[0];
  int x = 0;
  while (cmd[x]) {
    if (cmd[x] >= 'a' && cmd[x] <= 'z') {
      cmd[x] -= ('a' - 'A');
    }
    ++x;
  }

  // Search for command
  x = 0;
  int cmd_idx = -1;
  while (m_commands[x].m_name) {
    if (!strcmp(cmd, m_commands[x].m_name)) {
      // Trovato!
      cmd_idx = x;
      break;
    }
    ++x;
  }

  if (cmd_idx >= 0) {
    // Esegue il comando
    if (m_commands[cmd_idx].m_func) {
      int err = m_commands[cmd_idx].m_func(argc, argv);
      if (!err) {
	Serial.println("*** OK");
      }
      else {
	Serial.print("*** ERR:");
	Serial.println(err);
      }
    }
    else {
      Serial.println("To be done...");
    }
  }
  else {
    if (!strcmp(cmd, "HELP") || !strcmp(cmd, "?")) {
      terminal_help();
    }
    else {
      Serial.println("*** INVALID");
    }
  }

  clear_rx();
}

// void Terminale::doWorkAndAnswer(char* cmd)
// {
//   Serial.println("--------------------");
//   char* argv[8];
//   int argc = argSegment(cmd, argv, 8);

//   for (int i = 0; i < argc; ++i) {
//     Serial.print(i);
//     Serial.print(" -> '");
//     Serial.print(argv[i]);
//     Serial.println("'");
//   }

//   terrminale_help();
//   return 0;
// }

////////////////////////////////////////////////////////////////////////////////
void Terminale::terminal_help()
{
  Serial.println("-----------------------------------------------------");
  Serial.println("List of available commands:");
  int i = 0;
  while (m_commands[i].m_name) {
    //        if (is_command_accessible(state, list + i)) {
    Serial.print(m_commands[i].m_name);
    int spc = 8 - strlen(m_commands[i].m_name);
    while (spc > 0) {
      Serial.print(' ');
      --spc;
    }
    Serial.print(" : ");
    if (m_commands[i].m_help) {
      Serial.print(m_commands[i].m_help);
    }
    Serial.println("");
    ++i;
  }
  //    }
}
