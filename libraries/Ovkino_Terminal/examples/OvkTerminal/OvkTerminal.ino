//-----------------------------------------------------------------------
// Author: Ivan Zoli 2022
//-----------------------------------------------------------------------
// Terminal example
//-----------------------------------------------------------------------

#include <terminale.h>

static int g_param=0; //Parameter

//-----------------------------------------------------------------------
static int prm_inc(int argc, char** argv)
{
  int delta = 1;
  if (argc > 1) {
    delta = atoi(argv[1]);
  }

  g_param += delta;
  Serial.print("g_param: ");
  Serial.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
static int prm_dec(int argc, char** argv)
{
  int delta = 1;
  if (argc > 1) {
    delta = atoi(argv[1]);
  }

  g_param -= delta;
  Serial.print("g_param: ");
  Serial.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
static int prm_val(int argc, char** argv)
{
  int delta = 1;
  if (argc > 1) {
    g_param = atoi(argv[1]);
  }

  Serial.print("g_param: ");
  Serial.println(g_param);
  return 0;
}

//-----------------------------------------------------------------------
void stampa_help(const TermCmd* cmd)
{
  Serial.println("-----------------------------------------------------");
  Serial.println("List of available commands:");
  int i = 0;
  while (cmd[i].m_name) {
    Serial.print(cmd[i].m_name);
    int spc = 8 - strlen(cmd[i].m_name);
    while (spc > 0) {
      Serial.print(' ');
      --spc;
    }
    Serial.print(" : ");
    if (cmd[i].m_help) {
      Serial.print(cmd[i].m_help);
    }
    Serial.println("");
    ++i;
  }
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
static Terminale g_term(g_term_cmd);

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
