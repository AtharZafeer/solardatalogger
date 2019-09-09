// This #include statement was automatically added by the Particle IDE.
#include "ParticleFtpClient.h"
#include <time.h>

using namespace particleftpclient;

String hostname = "13.232.193.36"; //condifential
String username = "eira-ftp";
String password = "Ftp@2.0$$#";
int port = 21;

int timeout = 5;

ParticleFtpClient ftp = ParticleFtpClient();

void readPasvStream() {
  int count = 0;
  while (ftp.data.connected()) {
    while (ftp.data.available()) {
      char c = ftp.data.read();
      count++;
      Serial.print(c);
    }
  }
}

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
    float data[5] = {21, 1, 2, 4, 3};
    char data2[5];
    // Test writing a file
    Serial.println("TYPE A (text file)");
    if (!ftp.type("A")) {
      Serial.println("Couldn't set file type");
    } else if (!ftp.stor("test11.csv")) {
      Serial.println("Couldn't STOR test.csv");
    } else {
      Serial.println("STOR test.csv");
      for (int i = 0; i < 5; i++) {
        //char gcvt(data[i], 2, data2[i]);
        data2[i] = data[i];
        ftp.data.write(data2[i]);
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
