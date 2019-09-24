#include "Particle.h"     //library to run arduino code for particle board
#include "SdFat.h"        //library for memory card
#include"ParticleFtpClient.h" //library for FTP client
#include <OneWire.h>          //library for temperature sensor
#include <spark-dallas-temperature.h>
/*debug line*/
//#define DEBUG 1
/*debug line*/
#define battVolt B2 //battery voltage input analog pin
#define UCPVolt B3 //uncleaned panel voltage input analog pin
#define CPVolt B4 // cleaned panel voltage input analog pin
#define UCPcurr A1 //current uncleaned panel input analog pin
#define CPcurr A0 // current cleaned panel input analog pin
#define Temp D1 //temperature (no initialization need for temperature)
#define irr B5 //irradiation sensor input analog pin
#define relay C0 // relay Output analog pin
// pins assignment for various sensors

SdFat sd;       // initialization of sd card. we will be using this object of sd
//to call sd functions
const uint8_t chipSelect = A2; //chipSelect for SD, this can be anything, i used the default one
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
File myFile;  //This is the file object for SD card.
//The above three var declaration is for sd card function

OneWire oneWire(Temp);   //this is object function of temperature sensor
DallasTemperature sensors(&oneWire); //this is the object that uses the previous object
DeviceAddress batAddress,ucpAddress,cpAddress; //variable to store address of the temperature sensor
const int address[] = {0x33,0x81,0xF9};
/*The above are the last address bit (LSB) of each sensor. if you add a new sensor, be sure change
address to the new sensor's last address byte*/


using namespace particleftpclient;// dont forget this, there is a func of
// the dirctectory and its messing the naming convention
ParticleFtpClient ftp = ParticleFtpClient();  //ftp is the object for FTP client
String hostname = "Ip"; //condifential
String userName = "name"; //condifential
int port = 21; //condifential
int timeout = 5;
String password = "password"; //condifential
char name[20];
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
float UCPcurrentRef = 0;
float CPcurrentRef = 0;
float  currentRef = 0;
//current function variable declarations
float R1 = 56000.0; //resistance value of the voltage sensor
float R2 = 8200.0; // resistance value of the voltage sensor
float  vin = 0;
float vout = 0;
//voltage function variable declarations
/*none of the variables are declared inside the function because in the long run
declaring the variables inside a function will create a heap of unused memory that is
never modified more than once.
*/
//program begins
void setup() {
  /*
    This function will execute first. The IF loops that are here, will check if the particular sensor is attached or not
    this will first check the sensor conditions and will make the particular flag of the sensor 0 or 1. depending on
    this flag, whether a particular data from a sensor must be sent or not will be determined. if the sensor is not
    connected, the data needn't be sent. thus saving unnecessary data usage.
  */
  pinMode(battVolt, INPUT);
  pinMode(UCPVolt, INPUT);
  pinMode(CPVolt, INPUT);
  pinMode(UCPcurr, INPUT);
  pinMode(CPcurr, INPUT);
  pinMode(irr, INPUT);
  pinMode(relay, OUTPUT);
  //Serial.begin(9600);
  delay(100);
  sensors.begin(); // begin communication for temperature sensor
  //the below if statements set the flag
  if(voltageCheck(battVolt)) {
      flags[0] = 1;
    #ifdef DEBUG
      Serial.println("voltage bat available");
    #endif
  } else {
      flags[0] = 0;
    #ifdef DEBUG
      Serial.println("voltage bat unavailable");
    #endif
  }delay(100);

  if(voltageCheck(UCPVolt)) {
      flags[1] = 1;
    #ifdef DEBUG
      Serial.println("voltage ucp available");
    #endif
  } else {
      flags[1] = 0;
    #ifdef DEBUG
      Serial.println("voltage ucp unavailable");
    #endif
  }delay(100);

  if(voltageCheck(CPVolt)) {
    flags[2] = 1;
    #ifdef DEBUG
      Serial.println("voltage cp available");
    #endif
  } else {
    flags[2] = 0;
    #ifdef DEBUG
      Serial.println("voltage cp unavailable");
    #endif
  }delay(100);

  if(currentCheck(UCPcurr)) {
    flags[3] = 1;
    #ifdef DEBUG
      Serial.println(" UCPcurr available");
    #endif
  } else {
    flags[3] = 0;
    #ifdef DEBUG
      Serial.println("UCPcurr unavailable");
    #endif
  }delay(100);

  if(currentCheck(CPcurr)) {
    flags[4] = 1;
    #ifdef DEBUG
      Serial.println("CPcurr available");
    #endif
  } else {
    flags[4] = 0;
    #ifdef DEBUG
      Serial.println("CPcurr unavailable");
    #endif
  }delay(100);
 tempcheck(); // this function checks the flag and sensor within itself

  if(voltageFunc(irr)) {
    flags[8] = 1;
    #ifdef DEBUG
      Serial.println("irradiation sensor available");
    #endif
  } else {
    flags[8] = 0;
    #ifdef DEBUG
      Serial.println("irradiation sensor unavailable");
    #endif
  }delay(100);

  if(sdcheck()) {
    flags[9] = 1;
    #ifdef DEBUG
      Serial.println("sd card available");
    #endif
  } else {
    flags[9] = 0;
    #ifdef DEBUG
      Serial.println("sdcard unavailable");
    #endif
  }delay(100);


  utc = Time.now();//initialize time from server (will cost data)
  delay(1000);
}

//THIS FUNCTIONS GETS EXCUTED ALL THE TIME
void loop() {
  #ifdef DEBUG
    Serial.println("I am in loop function");
  #endif
  vBat = voltageFunc(battVolt); //read the battery voltage and set the flag high if it is  not zero
  if(vBat !=0) {
    flags[0] = 1;
    #ifdef DEBUG
      Serial.printf("\n voltage of bat: %.2f", vBat);
    #endif
  }else {
    flags[0]=0;
    #ifdef DEBUG
      Serial.println("voltage of bat unavailable");
    #endif
  }
  delay(10);

  vUcp = voltageFunc(UCPVolt);  //read the uncleaned panel voltage and set the flag high if its not zero
  if(vUcp != 0) {
    flags[1] = 1;
    #ifdef DEBUG
      Serial.printf("\n voltage of UCP: %.2f", vUcp);
    #endif
  } else {
    flags[1] = 0;
    #ifdef DEBUG
      Serial.println("voltage of ucp unavailable");
    #endif
    }
  delay(10);

  vCp = voltageFunc(CPVolt); //read the cleaned panel voltage and set the flag high if its not zero
  if(vCp != 0 ) {
    flags[2] = 1;
    #ifdef DEBUG
      Serial.printf("\n voltage of CP: %.2f", vCp);
    #endif
  } else {
    flags[2]=0;
    #ifdef DEBUG
      Serial.println("voltage of cp unavailable");
    #endif
  }
  delay(10);

  curUcp = currentFunc(UCPcurr); //read the uncleaned panel current and set the flag high if not zero
  if(currentCheck(A1)) {
    flags[3] = 1;
    #ifdef DEBUG
      Serial.printf("\n current of UCP: %.2f", curUcp);
    #endif
  } else {
    flags[3] =0;
    #ifdef DEBUG
      Serial.println("current of ucp unavailable");}
    #endif
  delay(10);

  curCp = currentFunc(CPcurr);   //read the cleaned panel current and set the flag high if not zero
  if(currentCheck(A0)) {
    flags[4] = 1;
    #ifdef DEBUG
      Serial.printf("\n current of cp: %.2f ", curCp);
  } else {
    #endif
    flags[4] = 0;
    #ifdef DEBUG
      Serial.println("current of cp unavailable");
    #endif
    }
  delay(10);
  tempFunc(); //this function takes the data from all the three temperature sensor using the onewire library

  delay(10);
  irradiation = voltageFunc(irr); //read the irradiation sensor voltage and set the flag high if non zero
  irradiation*=600;
  if(irradiation!=0) {
    flags[8]=1;
    #ifdef DEBUG
      Serial.printf("\n irradiation level : %.2f",irradiation);
    #endif
  }else {
    flags[8] =0;
    #ifdef DEBUG
      Serial.println("irradiation level: unavailable..");
    #endif
    }

  delay(10);
  now = millis(); //getting the time since the program started. This will return a value in milliseconds
  if ((now - lastTime) >= 1000) {  //this is to prevent timer overflow.. it should, in theory... the timer will overflow every 59days
    lastTime = now;                 //this will make it into a loop and prevent overflow
    Times = utc+(now/1000);
    Times+=19800; //utc+ist
    hrs = Times%(24 * 3600) / 3600; //calculate hrs from utc+ist
    mins = Times%(3600) / 60; //calculate mins from utc+ist
    sec = Times%60; //calculate sec from utc+ist
    #ifdef DEBUG
      Serial.printf("ist time in sec : %lu", Times);
    #endif
    if(hrs >= 11 && hrs <= 14 ) {
      digitalWrite(relay, LOW);  //Switch on the heat sink connected to the relay at these hours 11am to 2pm
      #ifdef DEBUG
        Serial.println("Relay is connected to heat sink");
      #endif
    }else {
      digitalWrite(relay, HIGH); //Switch off the relay at other time
      #ifdef DEBUG
        Serial.println("Relay is connected to battery");
      #endif
    }
    if(sec >= 0 && sec <=5 ) { //send data to ftp server and write the data on sd card
      WriteFtpServer(vBat, vUcp, vCp, curUcp, curCp, tempBat, tempUcp, tempCp,irradiation);
      delay(10);
      writeSDcard(vBat, vUcp, vCp, curUcp, curCp, tempBat, tempUcp, tempCp, irradiation);
    }
    #ifdef DEBUG
      Serial.printlnf("actualTIme: %d : %d : %d",hrs,mins,sec );
    #endif
  }
  //send data only if its after one minute
  #ifdef DEBUG
    Serial.print("flags: ");
    for(i = 0; i <=10; i++) {Serial.print(flags[i]);}
  #endif
}

//functions
//current function
float currentFunc( int pin) {
  if(pin == UCPcurr) {currentRef = UCPcurrentRef;}
  if(pin == CPcurr) {currentRef = CPcurrentRef;}
  for( i =0; i < 3000 ; i++)
  {
  currentValue+= (3.3/4095)*analogRead(pin); //currentPin4uncleanedpanel
  //the current is added we read the value for 30000 times continously
  }
  currentValue=currentValue/3000; //taking average, this will reduce the offset errors
  #ifdef DEBUG
    Serial.println("current raw Value");
    Serial.println(currentValue);
  #endif
  if(currentValue >= currentRef-2 && currentValue <= currentRef+2) {  //the value of the current ref,
    currentValue = currentRef;  //is based the power source given to power the current sensor when there is no current flowing throw it
  }
  current = ((currentValue - currentRef )/0.066); //divide the error by sensitivty we get the acutal current
  return current;
}


//voLtage function
float voltageFunc( int pin) {
// the voltage is nothing but a voltage divider circuit, since we know the resistance we can easily calculate the voltage
  for ( i = 0; i < 100; i++) {
  vout = (3.3/4095)*analogRead(pin); //use pins B2, B3, B4, B5
  vin += (vout / (R2/(R1+R2)) ); //remove the offset
}
  vin/=100;   //taking the average of all the readings
  return vin;
}

//temperature function
void tempFunc() {
  sensors.requestTemperatures();    //request data from the sensors
  for(int i = 0;i < 3; i++)     //loop through the three different address to
  {                       //find the right one and assign it accordingly. for example
     if(batAddress[7] == address[i]) { //this line checks battery temperature sensor's last address
     tempBat = sensors.getTempC(batAddress); //this address is passed and battery temperature is
     #ifdef DEBUG
       Serial.println("Battery Temperature:"); //if it matches as per the data we gave
       printAddress(batAddress);   //this address is then is for battery temperature
       Serial.printf("\t Temp C: ");    //assigned to the tempBat variable
       Serial.println(tempBat);Serial.println("\n");
      #endif
     }
     if(ucpAddress[7]==address[i]) {//the same is repeated three times. (3 sensors )
       tempUcp = sensors.getTempC(ucpAddress);
      #ifdef DEBUG
       Serial.println("UCP Temperature:");
       printAddress(ucpAddress);
       Serial.printf("\t Temp C: ");
       Serial.println(tempUcp);Serial.println("\n");
      #endif
     }
     if(cpAddress[7]==address[i]) {
       tempCp = sensors.getTempC(cpAddress);
      #ifdef DEBUG
       Serial.println("CP Temperature:");
       printAddress(cpAddress);
       Serial.print("\t tempCp C: ");
       Serial.println(tempCp);Serial.println("\n");
      #endif
     }
   }
}

// Sdcard writter
void writeSDcard(float vBat,float vUcp,float vCp,float curUcp,float curCp,float tempBat,float tempUcp,float tempCp, float irradiation){
  /*
    The sdfat library is a bit crude but is very reliable if you know how to use it properly
    If the schmatics is followed properly, then the read/write process of a memory card can be easily done, so make sure all the connections are
    proper and reliable.
    The sdfat library uses the SPI communication method. so the e Series board as pin dedicated for SPI communication
    // Primary SPI with DMA
    // SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)

  */
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {  //this will set the chipSelect pin and will communicate with the memory card in fullspeed.
    #ifdef DEBUG                               //half speed is not recommended, the library is crude and i disabled the half speed to prevent errors
      Serial.println("failed to open card");
    #endif
    return;
  }
  if (!myFile.open("data.csv", O_RDWR | O_CREAT | O_AT_END)) { //data.csv is the file name. O_RDWR is for reading and Writing of a already existing file
                                                              // O_CREAT is for creating the file if not already done and O_AT_END is to keep the cursor at the end of the file
    #ifdef DEBUG                                                          //so that the next write instruction doesnt overwrite.
		  Serial.println("opening test.txt for write failed");
    #endif
    return;
	}
  #ifdef DEBUG
    Serial.print("Writing to data.csv..."); //writes the data in the sdcard
  #endif

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
#ifdef DEBUG
  Serial.println("i am done with sdcard. data loaded!! ;^)");
#endif
}


// this functions sends data to ftp server (ONLY SEND)
/*
  The major flaw of this function is that it needs only strings to write data to the ftp
  so, the data2 variable with 12 character (DONT TOUCH THE variable SIZE, NEVER MAKE IT LOWER)
  the data2 variable acts a buffer for data type conversion. All the data type (float, int etc)
  are converted to character inorder to send data to ftp server using this library
*/
void WriteFtpServer(
  float vBat,float vUcp,float vCp,float curUcp,float curCp,float tempBat,
  float tempUcp,float tempCp,float irradiation
) {

  snprintf(name, sizeof(name), "%lu.csv",Times);   //this line does the datatype conversion, any data to character
  /*name variable holds the name of the file, in ftp server, it is data consuming to open the file, edit it. so, it is easier to
  create a new file everytime with a unique name(i have used utc time )*/
  #ifdef DEBUG
    Serial.println("name of the file :");
    Serial.print(name);
    Serial.println("Hello hoomans, i am working on ftp ! be patient v.v");
  #endif
  if(ftp.open(hostname, port, timeout)){   //opens the port and send the creditentials
    flags[10] =0; //seting flags B^)
    #ifdef DEBUG
      Serial.println("the server is open now");
    #endif
    if(ftp.user(userName)) {  //authenticating stuffs like creditentials
      flags[10] =1;
      #ifdef DEBUG
        Serial.println("Right user name");
      #endif
      if(ftp.pass(password)) {
        flags[10] =1;
        #ifdef DEBUG
          Serial.println("right password");
        #endif
      } else {
        flags[10] =0;
        #ifdef DEBUG
          Serial.println("wrong password, I guess");
        #endif
      }
    }else {
      flags[10] =0;
      #ifdef DEBUG
        Serial.println("wrong user name");
      #endif
    }
  } else {
    flags[10] =0;
    #ifdef DEBUG
      Serial.println("could'nt open check again");
    #endif
  }
  //we move to the needed dirctectory
  if(ftp.cwd("Xentra")){  //this navigates to a particular directory, Xentra can be replaced with a path
      #ifdef DEBUG
        Serial.println(ftp.pwd());
    //  Serial.println("I am in master :D ");
        Serial.println("Preparing to send a file to the server");
      #endif
      if(ftp.type("A")) {   // we set the file type, A for ascii B for binary.
        /* NOTE: A for ascii character for transfering numbers, texts etc
            B for binary character for transfering images etc
        */
        #ifdef DEBUG
          Serial.println("File type has been set");
          Serial.println(name);
        #endif
        if(ftp.stor(name)) { // we give name and extentsion. THis also creates the file
          #ifdef DEBUG
            Serial.println("the file has been created successfully");
          #endif
          ftp.data.write("\n");
          //write data here
          if (flags[0] != 0) {
            /*
              this will send data only if the flag is high (i.e the sensor is working) else, it
              won't send. The same applies for all the if statement in this function
            */
              snprintf(data2, sizeof(data2), "%.2f",vBat);      //convert the battery voltage into string to send
              ftp.data.write(data2);//send the data to ftp server
              ftp.data.write("\t");   //Battery voltage data will be sent every minute of the day
            }else { ftp.data.write("NC"); }

          if(hrs >= 4 && hrs <=19) {    // The rest of the data will be sent only in between these hours
              if(flags[1]!=0 )  {
               snprintf(data2, sizeof(data2), "%.2f",vUcp);
               ftp.data.write(data2);
               ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[2]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",vCp);
                ftp.data.write(data2);
                ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[3]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",curUcp);
                ftp.data.write(data2);
                ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[4]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",curCp);
                ftp.data.write(data2);
                ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[5]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",tempUcp);
                ftp.data.write(data2);
                ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[6]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",tempCp);
                ftp.data.write(data2);
                ftp.data.write("\t");
              }else { ftp.data.write("NC"); }

              if(flags[7]!=0 )  {
                snprintf(data2, sizeof(data2), "%.2f",tempBat);
                ftp.data.write(data2);
              }else { ftp.data.write("NC"); }

              if(flags[8]!=0 )   {
                snprintf(data2, sizeof(data2), "%.2f",irradiation);
                ftp.data.write(irradiation);
              }else { ftp.data.write("NC"); }
          }

          snprintf(data2, sizeof(data2), "%d : %d : %d",hrs, mins, sec); //time will also be sent
          ftp.data.write(data2);  //every min of the day
          #ifdef DEBUG
            Serial.println(data2);
            Serial.println("the data has been written successfully");
          #endif
          delay(100);
          ftp.data.flush(); // important, dont forget to flush!!
          if (!ftp.finish()) {
            #ifdef DEBUG
              Serial.println("Couldn't stop file upload");
            #endif
          }
        }else{
          #ifdef DEBUG
            Serial.println("couldn't create the file");
          #endif
        }
      }else{
        #ifdef DEBUG
          Serial.println("couldnt set the type ;_;");
        #endif
      }
  }else {
    #ifdef DEBUG
      Serial.println("Could'nt relocate");
    #endif
  }
  //delay(5000);
  Serial.println("QUIT");
  if (!ftp.quit()) {
    #ifdef DEBUG
      Serial.println("Couldn't quit FTP");
    #endif
  } else {
    #ifdef DEBUG
      Serial.println("Goodbye!");
    #endif
  }


}
//function to print address of the temperature sensor
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }

}
// checking functions

int voltageCheck(int pin) { //this functions will return 1 if the voltage sensor
 if(voltageFunc(pin)) { //has any value. else it will return 0
   return 1;
 }else {
   return 0;
 }
}
//voltage check
int currentCheck(int pin) { //this function will return 1 if there is current, else it will return 0
  float err = (3.3/4095)*analogRead(pin);
    if( err ) {
      for (i = 0; i< 1000; i++ ) {
        if(pin == UCPcurr){ UCPcurrentRef+= (3.3/4095)*analogRead(pin);}
        if(pin == CPcurr){CPcurrentRef+=(3.3/4095)*analogRead(pin);}
        }
        UCPcurrentRef/=1000;
        CPcurrentRef/=1000;
        #ifdef DEBUG
          Serial.printf("current reference of ucp: %.2f",UCPcurrentRef);
          Serial.printf("current reference of cp: %.2f",CPcurrentRef);
        #endif
      return 1;
    }else {
      currentRef =0;
      return 0;
    }
}
//current check

void tempcheck() {

    if(sensors.getDeviceCount() == 3){ //gets the device count and checks if it is
      flags[6] = 1;       //equal to 3, else makes the flag go to zero
      flags[5] = 1;
      flags[4] = 1;

    }else {
      flags[6] = 0;
      flags[5] = 0;
      flags[4] = 0;
    }

    //below we assign the address. it's out of if block because, if the count is not equal
    //to 3 it wont assign the address of even the working sensor.
    if (!sensors.getAddress(batAddress, 0)) {
      #ifdef DEBUG
        Serial.println("Unable to find address for batt temp");
      #endif
    } else {printAddress(batAddress);}
    if (!sensors.getAddress(ucpAddress, 1)) {
      #ifdef DEBUG
        Serial.println("Unable to find address for ucp temp ");
      #endif
    } else {printAddress(ucpAddress);}
    if (!sensors.getAddress(cpAddress, 2)) {
      #ifdef DEBUG
        Serial.println("Unable to find address for cp temp ");
      #endif
    }else {printAddress(cpAddress);}
    sensors.setResolution(batAddress,9); //we set the precision
    sensors.setResolution(ucpAddress,9);// 9bit or 11 bit or 12 bit
    sensors.setResolution(cpAddress,9);
}
//temp check

int sdcheck() { //this will check whether the SD card is there are not.
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    #ifdef DEBUG
      Serial.println("failed to open card");
    #endif
    return 0;
  }else {
    return 1;
  }
}
//SD card check

// irradiation check
