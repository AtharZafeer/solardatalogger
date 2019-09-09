#include "Particle.h"

// dependencies.SdFat=0.0.7
#include "SdFat.h"


// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = A2;

const unsigned long TEST_INTERVAL_MS = 10000;
unsigned long lastTest = 0;


void tryMeFirst();


void setup() {
	Serial.begin();
}

void loop() {
	if (millis() - lastTest >= TEST_INTERVAL_MS) {
		lastTest = millis();
		tryMeFirst();
	}
}

void tryMeFirst() {
	File myFile;

	// Initialize the library
	if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
		Serial.println("failed to open card");
		return;
	}

	// open the file for write at end like the "Native SD library"
	if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
		Serial.println("opening test.txt for write failed");
		return;
	}
	// if the file opened okay, write to it:
	Serial.print("Writing to test.txt...");
	myFile.println("testing 1, 2, 3.");
	myFile.printf("fileSize: %d\n", myFile.fileSize());

	// close the file:
	myFile.close();
	Serial.println("done.");

	// re-open the file for reading:
	if (!myFile.open("test.txt", O_READ)) {
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
