/**
 * *****************************************************************************
 * \brief   Serial Terminal
 *
 * \Author  Ivan Zoli
 *****************************************************************************/

// Include only once
#ifndef TERMINAL_H
#define TERMINAL_H

////////////////////////////////////////////////////////////////////////////////
//                          I N C L U D E S                                   //
////////////////////////////////////////////////////////////////////////////////

// System Library Include
#include <stdint.h>
#include <stddef.h>

// User Library Include

// Application Local Include

/// Command function prototype
typedef int (*TermCmdFunc)(int argc, char** argv);
/// Flags
typedef uint8_t TermAccessFlg;    ///< Terminal access privilege mask

///
/// \brief Termina command
///
class TermCmd {
public:
    TermCmd(const char* name, const TermCmdFunc func, const TermAccessFlg access, const char* help="")
    : m_name(name)
    , m_func(func)
    , m_access(access)
    , m_help(help)
    {}
    
public:
    const char*         m_name;     ///< Command name
    const TermCmdFunc   m_func;     ///< Command function
    const TermAccessFlg m_access;   ///< Access control flags function
    const char*         m_help;     ///< Brief help string for command
};

///
/// \brief Terminale
///
class Terminale
{
public:
  Terminale(const TermCmd* comandi, size_t buff_sz=128);

  ///
  /// \brief Read linear buffer, parse and answer
  ///
  void doWorkAndAnswer();

protected:
  ///
  /// \brief clear Rx buffer
  ///
  virtual void clear_rx();
  ///
  /// \brief receive a command or a portion of it
  ///
  virtual bool rx_command();
  ///
  /// \brief get recived command
  ///
  virtual char* const get_command();


protected:
  ///
  /// \brief print help
  ///
  void terminal_help();
  ///
  /// \brief buffeer overflow error
  ///
  bool is_error() { return m_overflow; }
  
private:
  const TermCmd*  m_commands;       //! Commands array
  char*           m_rx_buffer=0;    //! Rx buffer
  size_t          m_rx_buff_sz;     //! Buffer size
  int             m_cnt;            //! Counter received chars
  bool            m_overflow;       //! Buffer overflow
  bool            m_rx_complete;    //! A frame is complete
};

///
/// \brief Fine della lista
///
#define TERMINALE_FINE_LISTA  { 0, 0, 0, 0 }


#endif
