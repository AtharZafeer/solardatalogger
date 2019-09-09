#include "Particle.h"
#include "SdFat.h"
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
int temp=0;
const uint8_t chipSelect = A2;
float values = 10.00;
float value2= 11.00;
void write();
void read();
void setup() {
	Serial.begin(9600);
  Serial.println("in setup now, i dont know why you make me do this part >_>");
}
void loop(){
  Serial.println("in loop now");
  Serial.println("initiating writing");
  write();
  Serial.println("done writing :3");
  delay(1000);
  Serial.println("initiating reading");
  read();
  Serial.println("done, monsiur");
	delay(5000);
}
File myFile;
void write(){
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("failed to open card");
    return;
  }
  if (!myFile.open("test.csv", O_RDWR | O_CREAT | O_AT_END)) {
		Serial.println("opening test.txt for write failed");
		return;
	}

  Serial.print("Writing to test.txt...");
	myFile.println("I am the night");
	myFile.printf("%d  %d %d", values,value2, temp++);

  myFile.close();
  Serial.println("i am done!! ;)");
}
void read() {
  if (!myFile.open("test.csv", O_READ)) {
		Serial.println("opening test.txt for read failed");
		return;
	}
	Serial.println("test.txt content:");

	// read from the file until there's nothing else in it:
	int data;
	while ((data = myFile.read()) >= 0) {
		Serial.write(data);
	}
	// close the file:
	myFile.close();
}
