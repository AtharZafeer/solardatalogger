// This #include statement was automatically added by the Particle IDE.
#include "ParticleFtpClient.h"
#include <time.h>
#include <math.h>

using namespace particleftpclient;

String hostname = "13.232.193.36"; //condifential
String username = "eira-ftp";
String password = "Ftp@2.0$$#";
int port = 21;

int timeout = 5;

ParticleFtpClient ftp = ParticleFtpClient();


void setup() {
    Serial.begin(9600);
    Serial.println("Hello world");

    // Connect to host and authenticate, or halt
    if (!ftp.open(hostname, port, timeout)) {
      Serial.println("Couldn't connect to ftp host");
      while (1);
    }
    if (!ftp.user(username)) {
      Serial.println("Bad username");
      while (1);
    }
    if (!ftp.pass(password)) {
      Serial.println("Bad password");
      while (1);
    }

    // Test directory commands
    //Serial.println("MKD ParticleFtpClient");
    if (!ftp.cwd("Xentra")) {
      Serial.println("CWD ParticleFtpClient");
      Serial.println("Couldn't change directories to /Xentra");
    } else {
      Serial.println("PWD (should be /ParticleFtpClient)");
      Serial.println(ftp.pwd());

    }
   float datas[5] = {2.7167, 3.1231, 12.23455, 14.022, 1.0222 };
   char data2[15];
    // Test writing a file
    Serial.println("TYPE A (text file)");
    if (!ftp.type("A")) {
      Serial.println("Couldn't set file type");
    } else if (!ftp.stor("test17.csv")) {
      Serial.println("Couldn't STOR test.csv");
    } else {
      for (int i = 0; i < 5; i++) {
        //char gcvt(data[i], 2, data2[i]);
        snprintf(data2, sizeof(data2), "%.2f", datas[i]);
        ftp.data.write(data2);
        delay(1000);
      }
      ftp.data.flush();

      if (!ftp.finish()) {
        Serial.println("Couldn't stop file upload");
      }
    }




    // Quit!
    Serial.println("QUIT");
    if (!ftp.quit()) {
      Serial.println("Couldn't quit FTP");
    } else {
      Serial.println("Goodbye!");
    }
}
