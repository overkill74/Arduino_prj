#include <float16.h>

int g_n_to_test = 10;
float g_test_numbers[] = {
  -0.0012311935f,
  10.000000f,
  100.00000f,
  10000.000f,
  -10.000000f,
  -100.00000f,
  3124.0000f,
  0.00032305717f,
  -1421.0000f,
  (1.0f) / (0.0f),
};

_f16 g_test_numbers_f16[] = {
  0x950b,
  0x4900,
  0x5640,
  0x70e2,
  0xc900,
  0xd640,
  0x6a1a,
  0x0d4b,
  0xe58d,
  0x7c00
};

void setup()
{
  Serial.begin(115200);
  Serial.println("TestApp per F16 library");
}

void loop()
{
  int x;

  Serial.println("TestApp per F16 library");

  for (x = 0; x < g_n_to_test; ++x) {
    _f16 tmp_f16 = floatToF16(g_test_numbers[x]);
    float tmp_float = f16ToFloat(tmp_f16);
    Serial.print(g_test_numbers[x]);
    Serial.print(" -> ");
    Serial.println(tmp_float);
  }
  Serial.println("Done!");

  delay(1000);
}
