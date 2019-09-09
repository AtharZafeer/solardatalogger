void setup() {
  pinMode(B5, OUTPUT);
  pinMode(B4, OUTPUT);
  digitalWrite(B4, HIGH);
}
void loop() {
  digitalWrite(B5, HIGH);
  delay(5000);
  digitalWrite(B5, LOW);
  delay(5000);
}
