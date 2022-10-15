////////////////////////////////////////////////////////////////////////////////
//                           I N C L U D E S                                  //
////////////////////////////////////////////////////////////////////////////////

// System Library Include
#include <Arduino.h>
#include <Stream.h>


// User Library Include
#include "termargs.h"
#include "terminale.h"

////////////////////////////////////////////////////////////////////////////////
//              P R I V A T E   S T A T I C   F U N C T I O N S               //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                     P U B L I C   F U N C T I O N S                        //
////////////////////////////////////////////////////////////////////////////////
Terminale::Terminale(const TermCmd* comandi, Stream& stream, size_t buff_sz)
  : m_commands(comandi)
  , m_stream(stream)
  , m_rx_buff_sz(buff_sz)
{
  if (m_rx_buff_sz < 16) {
    m_rx_buff_sz = 16;
  }
  m_rx_buffer = new char[m_rx_buff_sz];
  if (!m_rx_buffer) {
    m_stream.println("No buffer for terminal");
    m_rx_buff_sz = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Terminale::clear_rx()
{
  while (m_stream.available()) {
    m_stream.read();
  }
  m_cnt = 0;
  m_overflow = false;
  m_rx_complete = false;
}

////////////////////////////////////////////////////////////////////////////////
bool Terminale::rx_command()
{
  while (m_stream.available()) {
    char rxc = m_stream.read();
    if (m_cnt < (m_rx_buff_sz - 1)) {
      if (rxc >= ' ') {
        m_rx_buffer[m_cnt++] = rxc;
      }
    } else {
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
      int err = m_commands[cmd_idx].m_func(argc, argv, m_stream);
      if (!err) {
        m_stream.println("*** OK");
      } else {
        m_stream.print("*** ERR:");
        m_stream.println(err);
      }
    } else {
      m_stream.println("To be done...");
    }
  } else {
    if (!strcmp(cmd, "HELP") || !strcmp(cmd, "?")) {
      terminal_help();
    } else {
      m_stream.println("*** INVALID");
    }
  }

  clear_rx();
}

// void Terminale::doWorkAndAnswer(char* cmd)
// {
//   m_stream.println("--------------------");
//   char* argv[8];
//   int argc = argSegment(cmd, argv, 8);

//   for (int i = 0; i < argc; ++i) {
//     m_stream.print(i);
//     m_stream.print(" -> '");
//     m_stream.print(argv[i]);
//     m_stream.println("'");
//   }

//   terrminale_help();
//   return 0;
// }

////////////////////////////////////////////////////////////////////////////////
void Terminale::terminal_help()
{
  m_stream.println("-----------------------------------------------------");
  m_stream.println("List of available commands:");
  int i = 0;
  while (m_commands[i].m_name) {
    //        if (is_command_accessible(state, list + i)) {
    m_stream.print(m_commands[i].m_name);
    int spc = 8 - strlen(m_commands[i].m_name);
    while (spc > 0) {
      m_stream.print(' ');
      --spc;
    }
    m_stream.print(" : ");
    if (m_commands[i].m_help) {
      m_stream.print(m_commands[i].m_help);
    }
    m_stream.println("");
    ++i;
  }
  //    }
}