/* Data Logger */

#include "DHT.h"
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include "RTClib.h"
#include <DallasTemperature.h>
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>

#define DHTPIN 2     
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define ONE_WIRE_BUS 7
#define LED_PIN 8

RTC_PCF8523 rtc;

const int chipSelect = 10;
int photoSensorPin = A0;
int sensorValue = 0;

int logInterval = 10;  // 10 seconds 

File dataFile;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String dataString = "";

DHT dht(DHTPIN, DHTTYPE);

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup() {

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(57600);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) delay(10);
  }

   if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
  
  rtc.start();

  dht.begin();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  dataFile = SD.open("datalog.txt", FILE_WRITE);

  dataString = "Date, Time, Soil Temperature, Humidity, Temperature, PhotoSensor";
  
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }    // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop() {

  dataString = "";
  
  DateTime now = rtc.now();
  
  float humidity = dht.readHumidity();
  float temperature = (dht.readTemperature() * 9/5) + 32;

  sensorValue = analogRead(photoSensorPin);

  sensors.requestTemperatures();
  float soilTemp = (sensors.getTempCByIndex(0) * 9/5) + 32;


//  Serial.print(now.year());
//  Serial.print('/');
//  Serial.print(now.month());
//  Serial.print('/');
//  Serial.print(now.day());
//  Serial.print(" ");
//  Serial.print(now.hour());
//  Serial.print(':');
//  Serial.print(now.minute());
//  Serial.print(':');
//  Serial.print(now.second());
//  Serial.println();
//
//  Serial.print("Humidity - ");
//  Serial.println(humidity);
//  Serial.print("Temperature - ");
//  Serial.println(temperature);
//  Serial.print("Photo Sensor - ");
//  Serial.println(sensorValue);
//  Serial.println();

  dataString = dataString + now.month() + "/" + now.day() + "/" + now.year() + "," + now.hour() + ":" + now.minute() + ":" + now.second() + ",";
  dataString = dataString + soilTemp + ",";
  dataString = dataString + humidity + ",";
  dataString = dataString + temperature + ",";
  dataString = dataString + sensorValue;

  digitalWrite(LED_PIN, HIGH);

  dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }    // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  
  digitalWrite(LED_PIN, LOW);

  delay(logInterval * 1000);
  
}
