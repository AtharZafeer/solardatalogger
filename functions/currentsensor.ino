float currentFunc( int pin) {
    float currentValue=0;
    float current = 0;
    for(int i =0; i < 3000 ; i++)
  {
    currentValue+= (3.3/4095)*analogRead(pin); //currentPin4uncleanedpanel

  }
  //Serial.println(currentValue);

  currentValue=(currentValue/3000)*1000;
  Serial.println(currentValue);
  if(currentValue >= 2428 && currentValue <=2440) {
    currentValue = 2430;
  }
  current = ((currentValue - 2430 )/0.066);
  Serial.println("current : ");
  Serial.printf("\t%.2f\n",current);
  return current;
}
void setup() {
  Serial.begin(9600);
  currentFunc(A1);

}
void loop () {
  Serial.println("sensor 1 : ");
 currentFunc(A0);
 Serial.println("sensor 2 : ");
 currentFunc(A1);
 delay(500);
}
//float  vin = (vout / (R2/(R1+R2)) )

int currentCheck(int pin) {
  int temp = currentFunc(pin);
    if(temp || temp >13) {
      return 0;
    }else {
      return 1;
    }
}
