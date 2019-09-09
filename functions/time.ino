/* unsigned long int currentTime[4];
unsigned long lastTime = 0;
unsigned long int utc = 0;
void setup() {


    Serial.begin(9600);
    utc = Time.now();
    //Time.Sync();

}

void loop() {
  /*Serial.printf("\n%d : %d : %d", Time.hour(), Time.minute(), Time.second());
  delay(1000);
  if(Time.second() == 00) {
    Serial.println("its been a minute");
        //Serial.println(Time.now());
    currentTime[0] = utc;
    unsigned long int now = millis();
    currentTime[0]+=now;
      Serial.println(currentTime[0]);
      currentTime[3] = (currentTime[0]%(24 * 3600)) / 3600;       //hours
      currentTime[2] = currentTime[0]%(3600) / 60; //minutes
      currentTime[1] = currentTime[0]%60; // seconds
      Serial.println("i am in the condition");
      Serial.printf("\n %d : %d: %d \n",currentTime[3],currentTime[2],currentTime[1] );

/*    if ((now - lastTime) >= 1000) {
  		lastTime = now;
      currentTime[0]+=now;
      Serial.println(currentTime[0]);
      currentTime[3] = (currentTime[0]%(24 * 3600)) / 3600;       //hours
      currentTime[2] = currentTime[0]%(3600) / 60; //minutes
      currentTime[1] = currentTime[0]%60; // seconds
      Serial.println("i am in the condition");
      Serial.printf("\n %d : %d: %d \n",currentTime[3],currentTime[2],currentTime[1] );

  	}*/
    /*currentTime[0] = Time.now();
    currentTime[3] = (currentTime[0]%(24 * 3600)) / 3600;       //hours
    currentTime[2] = currentTime[0]%(3600) / 60; //minutes
    currentTime[1] = currentTime[0]%60; // seconds
    //Serial.printf("\n %d : %d: %d \n",currentTime[3],currentTime[2],currentTime[1] );
}*/
/*
unsigned long lastTime = 0;

void setup() {
	Serial.begin(9600);
}

void loop() {
	unsigned long now = millis();
	if ((now - lastTime) >= 1000) {
		lastTime = now;
		Serial.printlnf("%lu", now);
	}
}*/
/*
void time(){
  Time.zone(+5.30);
  hrs= Time.hour();
  mins = Time.minute();
  sec = Time.second();
}
*/
unsigned long int utc = 0;
unsigned long int now = 0;
unsigned long int lastTime = 0;
unsigned long int Times = 0;
int hrs, mins, sec;
void setup() {
  Serial.begin(9600);
  utc = Time.now();
}

void loop() {
  now = millis();


  if ((now - lastTime) >= 1000) {
		lastTime = now;
    Serial.printlnf("utc : %lu",utc);
    Serial.printlnf("now %lu", now);
    Times = utc+(now/1000);
    Times+=19800;
    hrs = Times%(24 * 3600) / 3600;
    mins = Times%(3600) / 60;
    sec = Times%60;
		Serial.printlnf("actualTIme: %d : %d : %d",hrs,mins,sec );
	}
  delay(1000);
}
