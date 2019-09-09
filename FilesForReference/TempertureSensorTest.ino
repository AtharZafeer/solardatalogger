#define IN A0
void setup() {
  pinMode(IN,INPUT);
  Serial.begin(9600);
}
void loop() {
  float voltage = (3.3/4095)*analogRead(IN);
  Serial.println(voltage,5);
  float R2 = 1000 * 1/(3.3/voltage - 1);

  Serial.print("Resistance "); Serial.println(R2,5);
  delay(1000);
  float temp = ((R2/100)-1)/(0.00385);
  Serial.print("temp");Serial.println(temp,5);
}
