#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D1
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
float temp = 0;
DeviceAddress bat,ucp,cp;
const int c[] = {0x33,0x81,0xF9};
void setup() {
  Serial.begin(9600);
  sensors.begin();
  Serial.print(sensors.getDeviceCount(), DEC);
  if (!sensors.getAddress(bat, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(ucp, 1)) Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(cp, 2)) Serial.println("Unable to find address for Device 2");
  sensors.setResolution(bat,TEMPERATURE_PRECISION);
  sensors.setResolution(ucp,TEMPERATURE_PRECISION);
  sensors.setResolution(cp,TEMPERATURE_PRECISION);
  printAddress(bat);
  printAddress(ucp);
  printAddress(cp);
}

void loop() {
  Serial.println("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
 for(int i = 0;i < 3; i++)
 {
    if(bat[7] == c[i]) {
      Serial.println("Battery Temperature:");
      printAddress(bat);
      temp = sensors.getTempC(bat);
      Serial.print("Temp C: ");
  Serial.println(temp);Serial.println("\n");
  }
  if(ucp[7]==c[i]) {
    Serial.println("UCP Temperature:");
    printAddress(ucp);
    temp = sensors.getTempC(ucp);
    Serial.print("Temp C: ");
  Serial.println(temp);Serial.println("\n");
  }
  if(cp[7]==c[i]) {
    Serial.println("CP Temperature:");
    printAddress(cp);
    temp = sensors.getTempC(cp);
      Serial.print("Temp C: ");
  Serial.println(temp);Serial.println("\n");
  }
  delay(1000);
}
}
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }

}


float tempFunc() {
  sensors.requestTemperatures();
  for(int i = 0;i < 3; i++)
  {
     if(bat[7] == c[i]) {
       Serial.println("Battery Temperature:");
       printAddress(bat);
       tempBat = sensors.getTempC(bat);
       Serial.print("Temp C: ");
   Serial.println(tempBat);Serial.println("\n");
   }
   if(ucp[7]==c[i]) {
     Serial.println("UCP Temperature:");
     printAddress(ucp);
     tempUcp = sensors.getTempC(ucp);
     Serial.print("Temp C: ");
   Serial.println(tempUcp);Serial.println("\n");
   }
   if(cp[7]==c[i]) {
     Serial.println("CP Temperature:");
     printAddress(cp);
     tempCp = sensors.getTempC(cp);
       Serial.print("tempCp C: ");
   Serial.println(tempCp);Serial.println("\n");
   }
}
