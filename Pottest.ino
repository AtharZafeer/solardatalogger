// You'll need to include the Adafruit DHT library from the libraries section
#include "Adafruit_DHT.h"

// This example assumes the sensor to be plugged into CONN2
#define DHTPIN D1     // what pin we're connected to

// Here we define the type of sensor used
#define DHTTYPE DHT11        // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {

    // We open up a serial port to monitor the sensor values
    Serial.begin(9600);
    Serial.println("DHT11 test!");

    dht.begin();
}

void loop() {
    // Wait a few seconds between measurements.
    delay(2000);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds
    float h = dht.getHumidity();
    // Read temperature as Celsius
    float t = dht.getTempCelcius();
    // Read temperature as Farenheit
    float f = dht.getTempFarenheit();

    // Check if any reads failed and exit early (to try again).
   if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print the data over serial
    Serial.print("Humid: ");
    Serial.print(h);
    Serial.print("% - ");
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print("*c ");
    Serial.println(Time.timeStr());

    // Publish data to the Particle cloud.
    // Remember that you'll consume data every time you publish to the cloud.
    //Particle.publish("temp", String (f));
    //Particle.publish("humi", String (h));
}
