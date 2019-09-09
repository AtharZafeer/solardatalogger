#include<OneWire.h>
#include<spark-dallas-temperature.h>
OneWire onewire(D3);
DallasTemperature dallas(&onewire);

void setup() {
	Serial.begin(9600);
	dallas.begin();
}

void loop() {
	  dallas.requestTemperatures();
    float celsius = dallas.getTempCByIndex( 0 );
    Serial.print("Temperature: ");
    Serial.println(celsius) ;
    delay(1000);
}
