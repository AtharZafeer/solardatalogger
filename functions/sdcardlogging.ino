#include "Particle.h"
#include "SdFat.h"
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = A2;
File myFile;
void write(){
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("failed to open card");
    return;
  }
  if (!myFile.open("data.csv", O_RDWR | O_CREAT | O_AT_END)) {
		Serial.println("opening test.txt for write failed");
		return;
	}

  Serial.print("Writing to test.txt...");
	myFile.println("I am the night");
	myFile.printf("%d \t %d \t %d "); //write the data you want to load in

  myFile.close();
  Serial.println("i am done!! ;)");
}

void setup() {
  Serial.begin();
  write();
}

int sdcheck() {
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("failed to open card");
    return 0;
  }else {
    return 1;
  }
}
