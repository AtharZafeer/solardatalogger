float currentValue=0;
float current=0;
void setup() {
  pinMode(A4, INPUT);
  Serial.begin(9600);
}
void loop() {
  for(int i =0; i < 150 ; i++)
  {
    currentValue+=  analogRead(A4);
    delay(5);
  }
  currentValue=(3.3/4095)*(currentValue)/150;

  Serial.print("voltage: ");Serial.println(currentValue,2);
  //float currentR = (currentValue);
  if(currentValue>= 1.61 && currentValue <= 1.63) {
    currentValue = 1.63;
    current = ((currentValue - 1.63 )/0.066);
  }else {
    current = ((currentValue - 1.63 )/0.066);
  }

  //current=current*1000;
//  current = truncator(current);
  Serial.println(current,2);

}
float truncator(float num) {
  int temp = num*100;
  temp/=100;
  return temp;
}
