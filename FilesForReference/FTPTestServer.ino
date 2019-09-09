#include"ParticleFtpClient.h"
#include <time.h>
using namespace particleftpclient;// dont forget this, there is a func of
// the dirctectory and its messing the naming convention

ParticleFtpClient ftp = ParticleFtpClient(); //initialize the constructor
String hostname = "13.232.193.36"; //condifential
String userName = "eira-ftp"; //condifential
int port = 21; //condifential
int timeout = 5;
String password = "Ftp@2.0$$#"; //condifential
String newWord = "Blessed are those who stand before the weak and wicked and donot faulter";
void setup() {
  Serial.begin(9600);
  Serial.println("Hello world, i am working !");
  if(ftp.open(hostname, port, timeout)){   //opens the port
    Serial.println("the server is open now");
    if(ftp.user(userName)) {  //authenticating stuffs like creditentials
      Serial.println("Right user name");
      if(ftp.pass(password)) {
        Serial.println("right password");
      } else {
        Serial.println("wrong password, I guess");
      }
    }else {
      Serial.println("wrong user name");
    }
  } else {
    Serial.println("could'nt open check again and ;_; suffer in silence");

  }
  //we move to the needed dirctectory
  if(ftp.cwd("Xentra")){
      Serial.println(ftp.pwd());
      Serial.println("I am in master :D ");
      Serial.println("Preparing to send a file to the server");
      if(ftp.type("A")) {   // we create the file
        Serial.println("File type has been set");
        if(ftp.stor("SecondFile.csv")) { // we give name and extentsion
          Serial.println("the file has been created successfully");
          ftp.data.write(newWord);
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
  delay(5000);
  Serial.println("QUIT");
  if (!ftp.quit()) {
    Serial.println("Couldn't quit FTP");
  } else {
    Serial.println("Goodbye!");
  }

}
void loop () {

}
