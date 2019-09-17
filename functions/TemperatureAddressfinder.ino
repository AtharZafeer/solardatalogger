
#include "DS18.h"
DS18 sensor(D1);  //for particle board any digital pin and change it here
//for arduino use any digital pin and change it here
float temperature[3];

void loop() {
  tempFunc();
  delay(2000);
}
void tempFunc() {

  if(sensor.read())
  {
    temperature[i] = sensor.celsius();
    uint8_t addr[8];
    sensor.addr(addr);
    Serial.printf(
      " ROM=%02X%02X%02X%02X%02X%02X%02X%02X",
      addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]
    );
     Serial.printf("\n Temperature %.2f C ",temperature[i] );
     delay(200);
  }else {
    Serial.println("temperature failed");
  }
  Serial.println("loop ends");

  return temperature;
}
