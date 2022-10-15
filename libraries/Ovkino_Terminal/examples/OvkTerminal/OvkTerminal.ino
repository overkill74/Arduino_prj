//-----------------------------------------------------------------------
// Author: Ivan Zoli 2022
//-----------------------------------------------------------------------
// Terminal example
//-----------------------------------------------------------------------

#include <terminale.h>

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

//-----------------------------------------------------------------------
void setup()
{
  // Serial
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("\n\n\nTerminal Test Starting");
}

  
//-----------------------------------------------------------------------
void loop()
{
  g_term.doWorkAndAnswer();
}
