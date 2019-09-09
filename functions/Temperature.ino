
#include "DS18.h"
DS18 sensor(D1);
float temperature[3];
/*  // Set up 'power' pins, comment out if not used!
float temp[3];
void setup() {
}
void loop() {
  for (int i =0; i < 3; i++) {
    if(sensor.read()) {

    temp[i] = sensor.celsius();
    Serial.printf("\nTemperature %.2f C \n", temp[i]);
    Serial.println(i);
    printDebugInfo();
    delay(200);
  }
  else {
    // Once all sensors have been read you'll get searchDone() == true
    // Next time read() is called the first sensor is read again
    if (sensor.searchDone()) {
      Serial.println("No more addresses.");
      // Avoid excessive printing when no sensors are connected
      delay(250);

    // Something went wrong
    } else {
      printDebugInfo();
    }
  }
}
  Serial.println("loop end");
}


void printDebugInfo() {
  // If there's an electrical error on the 1-Wire bus you'll get a CRC error
  // Just ignore the temperature measurement and try again
  if (sensor.crcError()) {
    Serial.print("CRC Error ");
  }

  // Print the sensor type
  const char *type;
  switch(sensor.type()) {
    case WIRE_DS1820: type = "DS1820"; break;
    case WIRE_DS18B20: type = "DS18B20"; break;
    case WIRE_DS1822: type = "DS1822"; break;
    case WIRE_DS2438: type = "DS2438"; break;
    default: type = "UNKNOWN"; break;
  }
  Serial.print(type);

  // Print the ROM (sensor type and unique ID)
  uint8_t addr[8];
  sensor.addr(addr);
  Serial.printf(
    " ROM=%02X%02X%02X%02X%02X%02X%02X%02X",
    addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]
  );

  // Print the raw sensor data
  uint8_t data[9];
  sensor.data(data);
  Serial.printf(
    " data=%02X%02X%02X%02X%02X%02X%02X%02X%02X",
    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]
  );
}
*/
void loop() {
  tempFunc();
  delay(2000);
}
void tempFunc() {
  for( int i =0;i < 3;i++) {
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
 }
  //return temperature;
}
