
const int pulpin = B5;
const int dirpin = B3;
//const int enpin = 8;

void setup() {
pinMode(pulpin, OUTPUT);
pinMode(dirpin, OUTPUT);
//pinMode(enpin, OUTPUT);
//digitalWrite(enpin, LOW);
//Serial.begin(9600);

}

void loop() {

digitalWrite(dirpin, HIGH);
for(int i = 0;i< 200;i++) {
    digitalWrite(pulpin, HIGH);
    delayMicroseconds(700);
    digitalWrite(pulpin, LOW);
    delayMicroseconds(700);
  }

 //analogWrite(pulpin, 110);
}
