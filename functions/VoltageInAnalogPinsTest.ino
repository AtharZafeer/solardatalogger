int pins[] = {D1,D2,D3,D4,D5};
void setup(){
  for(int i=0;i<5;i++) {
  pinMode(pins[i],OUTPUT);
  digitalWrite(pins[i],HIGH);
}}
