

class OneWire;
class DallasTemperature;

#define T_ERRROR  -100

class TWater
{

public:
  /// \brief Costruttore
  /// \param in pin_dati - data pin
  TWater(const int pin_dati);

  /// \brief Legge la temperatura dal sensore
  float readTemperature();

private:
  /// \brief Inizializza il sensore
  void init_sensor();

private:
  float m_old_temp = T_ERRROR;            //! vecchio valore di temperatura
  OneWire* m_oneWire = nullptr;           //! onewire pointer
  DallasTemperature* m_sensors = nullptr; //! dallas temperature
  const int m_pin_dati;                   //! Pin dati
};