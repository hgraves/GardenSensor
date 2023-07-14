#include "Arduino.h"

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);


#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress insideThermometer = { 0x28, 0x94, 0xE2, 0xDF, 0x02, 0x00, 0x00, 0xFE };
DeviceAddress outsideThermometer = { 0x28, 0x6B, 0xDF, 0xDF, 0x02, 0x00, 0x00, 0xC0 };
DeviceAddress dogHouseThermometer = { 0x28, 0x59, 0xBE, 0xDF, 0x02, 0x00, 0x00, 0x9F };


//The setup function is called once at startup of the sketch
void setup()
{
	  Serial.begin(115200);
	  Serial.println("DHTxx test!");

	  dht.begin();


	  // set the resolution to 10 bit (good enough?)
	  sensors.setResolution(insideThermometer, 10);
	  sensors.setResolution(outsideThermometer, 10);
	  sensors.setResolution(dogHouseThermometer, 10);

}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
  }


}

// The loop function is called in an endless loop
void loop()
{
	  // Reading temperature or humidity takes about 250 milliseconds!
	  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	  float h = dht.readHumidity();
	  float t = (dht.readTemperature() * 9 / 5) + 32;

	  // check if returns are valid, if they are NaN (not a number) then something went wrong!
	  if (isnan(t) || isnan(h)) {
	    Serial.println("Failed to read from DHT");
	  } else {
	    Serial.print("Humidity: ");
	    Serial.print(h);
	    Serial.print(" %\t");
	    Serial.print("Temperature: ");
	    Serial.print(t);
	    Serial.println(" *F");
	  }

	  delay(2000);
	  Serial.print("Getting temperatures...\n\r");
	  sensors.requestTemperatures();

	  Serial.print("Inside temperature is: ");
	  printTemperature(insideThermometer);
	  Serial.print("\n\r");
	  Serial.print("Outside temperature is: ");
	  printTemperature(outsideThermometer);
	  Serial.print("\n\r");
	  Serial.print("Dog House temperature is: ");
	  printTemperature(dogHouseThermometer);
	  Serial.print("\n\r\n\r");

}
