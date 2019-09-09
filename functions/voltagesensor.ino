float voltageFunc( int pin) {
  float R1 = 56000.0;
  float R2 = 8200.0;
  int vout = (3.3/4095)*analogRead(pin); //use pins B2, B3, B4, B5
  float  vin = (vout / (R2/(R1+R2)) )-0.01;
  Serial.print("Voltage:  "); Serial.println(vin);
  return vin;
}
void setup() {
  Serial.begin(9600);
  voltageFunc(B2);
}

int voltageCheck(int pin) {
 if(voltageFunc(pin)) {
   return 1;
 }else {
   return 0;
 }
}
