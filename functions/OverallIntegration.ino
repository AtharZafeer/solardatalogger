#include "DS18.h"
#include "Particle.h"
#include "SdFat.h"
#include"ParticleFtpClient.h"


#define battVolt B2 //battery voltage
#define UCPVolt B3 //uncleaned panel voltage
#define CPVolt B4 // cleaned panel voltage
#define UCPcurr A1 //current uncleaned panel
#define CPcurr A0 // current cleaned panel
#define Temp D1 //temperature of battery
#define irr B5 //irradiation sensor
#define relay C0 // relay
// pins assignment for various sensors

SdFat sd;
const uint8_t chipSelect = A2;
File myFile;
//The above three var declaration is for sd card function
DS18 sensor(Temp);
float temperature[5];

using namespace particleftpclient;// dont forget this, there is a func of
// the dirctectory and its messing the naming convention
ParticleFtpClient ftp = ParticleFtpClient();
String hostname = "13.232.193.36"; //condifential
String userName = "eira-ftp"; //condifential
int port = 21; //condifential
int timeout = 5;
String password = "Ftp@2.0$$#";
char name[20];
//String currentTime = ".csv";
//unsigned long int count = 0;
char data2[20];
// kept the condifential files open, the ftp lib is not accepting class or array
// the above var declaration are for ftp server function
//Functions list, also function declaration
void WriteFtpServer(float, float,float,float,float,float,float,float,float);
void writeSDcard(float, float,float,float,float,float,float,float,float);
float currentFunc( int );
float voltageFunc( int );
void tempFunc();
int sdcheck();
void tempcheck();
int currentCheck(int );
int voltageCheck(int );
int irrCheck(int );

//variable declarations
float vBat = 0;   //voltage of battery
float vUcp = 0;   //voltage of uncleaned panel
float vCp = 0;    //voltage of cleaned panel
float curUcp = 0; //current of uncleaned panel
float curCp = 0; //current of cleaned panel
float tempBat = 0; //temperature of battery
float tempUcp = 0; //temperature of uncleaned panel
float tempCp = 0; //temperature of cleaned panel
float irradiation = 0; //irradiation sensor value
unsigned long int utc = 0; //for time
unsigned long int now = 0; //for time
unsigned long int lastTime = 0; //for time
unsigned long int Times = 0; //for time
int hrs, mins, sec; //for time
int i;
bool flags[]={0,0,0,0,0,0,0,0,0,0,0};

/*
  flags[0] is for battery voltage sensor
  flags[1] is for voltage sensor of uncleaned panel
  flags[2] is for voltage sensor of cleaned panel
  flags[3] is for current of uncleaned panel
  flags[4] is for current of cleaned panel
  flags[5] is for temperature sensor of the  uncleaned panel
  flags[6] is for temperature sensor of the cleaned panel
  flags[7] is for the temperature sensor of the battery/ambient
  flags[8] is for the irradiation sensor
  flags[9] is for the sd detection
  flags[10] is for the ftp server working
*/


// function variable declarations
float currentValue=0;
float current = 0;
//current function variable declarations
float R1 = 56000.0;
float R2 = 8200.0;
float  vin = 0;
float vout = 0;
//voltage function variable declarations

//program begins
void setup() {
  pinMode(battVolt, INPUT);
  pinMode(UCPVolt, INPUT);
  pinMode(CPVolt, INPUT);
  pinMode(UCPcurr, INPUT);
  pinMode(CPcurr, INPUT);
  pinMode(irr, INPUT);
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  delay(100);
  if(voltageCheck(battVolt)){
      flags[0] = 1;    //
      Serial.println("voltage bat available");

  } else {
    flags[0] = 0;
    Serial.println("voltage bat unavailable");

  }delay(100);
  if(voltageCheck(UCPVolt)) {
    flags[1] = 1;Serial.println("voltage ucp available");
  } else {
    flags[1] = 0;Serial.println("voltage ucp unavailable");
  }delay(100);
  if(voltageCheck(CPVolt)) {
    flags[2] = 1;Serial.println("voltage cp available");
  } else {
    flags[2] = 0;Serial.println("voltage cp unavailable");
  }delay(100);
  if(currentCheck(UCPcurr)) {
    flags[3] = 1;Serial.println(" UCPcurr available");
  } else {
    flags[3] = 0;Serial.println("UCPcurr unavailable");
  }delay(100);
  if(currentCheck(CPcurr)) {
    flags[4] = 1;Serial.println("CPcurr available");
  } else {
    flags[4] = 0;Serial.println("CPcurr unavailable");
  }delay(100);
 tempcheck();
  if(voltageFunc(irr)) {
    flags[8] = 1;
  } else {
    flags[8] = 0;
  }delay(100);
  if(sdcheck()) {
    flags[9] = 1;Serial.println("sd card available");
  } else {
    flags[9] = 0;Serial.println("sdcard unavailable");
  }delay(100);


  utc = Time.now();//initialize time from server (will cost data)
  delay(1000);
}
void loop() {
  vBat = voltageFunc(battVolt); if(vBat !=0) {flags[0] = 1;}
  delay(10);
  vUcp = voltageFunc(UCPVolt); if(vUcp != 0) {flags[1] = 1;}
  delay(10);
  vCp = voltageFunc(CPVolt);  if(vCp != 0 ) {flags[2] = 1; }
  delay(10);
  curUcp = currentFunc(UCPcurr); if(curUcp != 0 && curUcp <= 30) {flags[3] = 1;}
  delay(10);
  curCp = currentFunc(CPcurr);  if(curCp != 0 && curUcp <= 30 ) {flags[4] = 1;}
  delay(10);
  tempFunc();
  tempBat = temperature[2]; if(tempBat!=0) {flags[7]= 1;}
  tempUcp = temperature[1]; if(tempUcp!=0) {flags[5] = 1;}
  tempCp = temperature[0]; if(tempCp!=0) {flags[6] = 1;}
  delay(10);
  irradiation = voltageFunc(irr); if(irradiation!=0) {flags[8]=1;}
  irradiation*=3000;
  delay(10);
  Serial.printf("\n %.2f \t %.2f \t%.2f \t%.2f \t%.2f \t%.2f \t%.2f \t%.2f \t%.2f \n", vBat, vUcp, vCp, curUcp, curCp, tempBat, tempUcp, tempCp, irradiation );
  delay(10);
  now = millis(); //getting the time since the program started
  if ((now - lastTime) >= 1000) {  //this is to prevent timer overflow.. it should, in theory... the timer will overflow every 59days
    lastTime = now;
    Times = utc+(now/1000);
    Times+=19800;
    hrs = Times%(24 * 3600) / 3600; //calculate hrs from utc+ist
    mins = Times%(3600) / 60; //calculate mins from utc+ist
    sec = Times%60; //calculate sec from utc+ist
    Serial.println(utc);
    if(sec >= 0 && sec <=5 ) {
      WriteFtpServer(vBat, vUcp, vCp, curUcp, curCp, tempBat, tempUcp, tempCp,irradiation);
      delay(10);
      writeSDcard(vBat, vUcp, vCp, curUcp, curCp, tempBat, tempUcp, tempCp, irradiation);
    }
    if(hrs >= 11 && hrs <= 14 ) {
      digitalWrite(relay, HIGH);

    }else {
      digitalWrite(relay, LOW);

    }

    Serial.printlnf("actualTIme: %d : %d : %d",hrs,mins,sec );
  }
  //send data only if its after one minute

}

//functions
//current function
float currentFunc( int pin) {

  for( i =0; i < 3000 ; i++)
  {
  currentValue+= (3.3/4095)*analogRead(pin); //currentPin4uncleanedpanel

  }
  //Serial.println(currentValue);

  currentValue=currentValue/3000;
  Serial.println(currentValue);
  if(currentValue >= 2.42 && currentValue <=2.44) {
    currentValue = 2.43;
  }
  current = ((currentValue - 2.43 )/0.066);
  Serial.print("current : ");
  Serial.println(current);
  return current;
}


//voLtage function
float voltageFunc( int pin) {

  for ( i = 0; i < 100; i++) {
  vout = (3.3/4095)*analogRead(pin); //use pins B2, B3, B4, B5
  vin += (vout / (R2/(R1+R2)) ); //remove the offset
}
  vin/=100;
  Serial.print("\n Voltage:  ");
  Serial.println(vin);

  return vin;
}

//temperature function
void tempFunc() {
  for( i =0;i < 3;i++) {
  if(sensor.read())
  {
    temperature[i] = sensor.celsius();
    Serial.printf("\n Temperature %.2f C ",temperature[i] );
     delay(20);
  }else {
    Serial.println("temperature failed");
  }
  Serial.println("loop ends");
 }
  //return temperature;
}

// Sdcard writter
void writeSDcard(float vBat,float vUcp,float vCp,float curUcp,float curCp,float tempBat,float tempUcp,float tempCp, float irradiation){
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("failed to open card");
    return;
  }
  if (!myFile.open("data.csv", O_RDWR | O_CREAT | O_AT_END)) {
		Serial.println("opening test.txt for write failed");
		return;
	}

  Serial.print("Writing to data.csv...");
	//myFile.println("voltage of battery :");
	myFile.printf("\n voltage of the battery: %.2f ", vBat); //write the data you want to load in
  myFile.printf("\t voltage of the uncleaned panel : %.2f", vUcp);
  myFile.printf("\t voltage of the cleaned panel : %.2f",vCp);
  myFile.printf("\t current of the uncleaned panel : %.2f",curUcp);
  myFile.printf("\t current of the cleaned panel: %.2f",curCp);
  myFile.printf("\t temperature of the battery : %.2f",tempBat);
  myFile.printf("\t temperature of the uncleaned panel: %.2f ",tempUcp);
  myFile.printf("\t temperature of the cleaned panel : %.2f",tempCp);
  myFile.printf("\t irradiation value : %.2f", irradiation);
  myFile.printf("\t time: %d : %d : %d \n", hrs,mins,sec);
  myFile.close();
  Serial.println("i am done!! ;)");
}


// this functions sends data to ftp server ONLY SEND
void WriteFtpServer(
  float vBat,float vUcp,float vCp,float curUcp,float curCp,float tempBat,
  float tempUcp,float tempCp,float irradiation
) {

  snprintf(name, sizeof(name), "%lu.csv",Times);
  Serial.println();
  Serial.println(name);
  Serial.println("Hello world, i am working !");
  if(ftp.open(hostname, port, timeout)){   //opens the port
    Serial.println("the server is open now");
    flags[10] =0; //seting flags B^)
    if(ftp.user(userName)) {  //authenticating stuffs like creditentials
      Serial.println("Right user name");
      flags[10] =1;
      if(ftp.pass(password)) {
        Serial.println("right password");
        flags[10] =1;
      } else {
        Serial.println("wrong password, I guess");
        flags[10] =0;
      }
    }else {
      Serial.println("wrong user name");
      flags[10] =0;
    }
  } else {
    Serial.println("could'nt open check again");
    flags[10] =0;
  }
  //we move to the needed dirctectory
  if(ftp.cwd("Xentra")){

      Serial.println(ftp.pwd());
    //  Serial.println("I am in master :D ");
      Serial.println("Preparing to send a file to the server");
      if(ftp.type("A")) {   // we create the file
        Serial.println("File type has been set");
        Serial.println(name);
        if(ftp.stor(name)) { // we give name and extentsion
          Serial.println("the file has been created successfully");
          ftp.data.write("\n");



          //write data here
          if (flags[0] != 0) {
            snprintf(data2, sizeof(data2), "%.2f",vCp);
            if(ftp.data.write(data2)){Serial.println("data written");}
           }else { ftp.data.write("NC"); }
          if(flags[1]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",vUcp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
          }else { ftp.data.write("NC"); }
          if(flags[2]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",vCp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
           }else { ftp.data.write("NC"); }
          if(flags[3]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",curUcp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
           }else { ftp.data.write("NC"); }
          if(flags[4]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",curCp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
         }else { ftp.data.write("NC"); }
          if(flags[5]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",tempUcp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
          }else { ftp.data.write("NC"); }
          if(flags[6]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",tempCp);
            if(ftp.data.write(data2)){Serial.println("data written");}
            ftp.data.write("\t");
         }else { ftp.data.write("NC"); }
          if(flags[7]!=0 )  {
            snprintf(data2, sizeof(data2), "%.2f",tempBat);
            if(ftp.data.write(data2)){Serial.println("data written");}
         }else { ftp.data.write("NC"); }
          if(flags[8]!=0 )   {
            snprintf(data2, sizeof(data2), "%.2f",irradiation);
            if(ftp.data.write(irradiation)){Serial.println("data written");}
         }else { ftp.data.write("NC"); }
          snprintf(data2, sizeof(data2), "%d : %d : %d",hrs, mins, sec);
          ftp.data.write(data2);
          Serial.println(data2);
          Serial.println("the data has been written successfully");
          delay(100);
          ftp.data.flush(); // important, dont forget to flush!!
          if (!ftp.finish()) {
            Serial.println("Couldn't stop file upload");
          }
        }else{
          Serial.println("couldn't create the file");

        }
      }else{
        Serial.println("couldnt set the type ;_;");
      }
  }else {
    Serial.println("Could'nt relocate");
  }
  //delay(5000);
  Serial.println("QUIT");
  if (!ftp.quit()) {
    Serial.println("Couldn't quit FTP");
  } else {
    Serial.println("Goodbye!");
  }


}

// checking functions

int voltageCheck(int pin) {
 if(voltageFunc(pin)) {
   return 1;
 }else {
   return 0;
 }
}
//voltage check
int currentCheck(int pin) {
  float err = (3.3/4095)*analogRead(pin);
    if( err ) {
      return 0;
    }else {
      return 1;
    }
}
//current check

void tempcheck() {
for( i = 0; i < 3; i ++)
  {
    if(!sensor.read()){
      if(i = 0 && sensor.celsius()) {flags[6] = 1;}
      if(i = 1 && sensor.celsius()) {flags[5] = 1;}
      if(i = 2 && sensor.celsius()) {flags[4] = 1;}

  }else {
      if(i = 0) {flags[6] = 1;}
      if(i = 1) {flags[5] = 1;}
      if(i = 2) {flags[4] = 1;}
    }
  }
}
//temp check

int sdcheck() {
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("failed to open card");
    return 0;
  }else {
    return 1;
  }
}
//SD card check

// irradiation check
