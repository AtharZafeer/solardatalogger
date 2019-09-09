#define raw_voltage B4
float vout =0;
float R1 = 56000.0;
float R2 = 8200.0;
void setup() {
  Serial.begin(9600);
  pinMode(raw_voltage, INPUT);
}

void loop() {
  vout = (3.3/4095)*analogRead(raw_voltage);
  Serial.println(vout);
  float  vin = (vout / (R2/(R1+R2)) )-0.01;
  delay(1000);
  Serial.print("Voltage:  "); Serial.println(vin);
}
