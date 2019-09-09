int led = D7;  // The on-board LED

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);    

  String temp = String(random(60, 80));
  Particle.publish("temp", temp, PRIVATE);
  delay(30000);

  digitalWrite(led, LOW);
  delay(30000);
}
