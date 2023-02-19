#include "SdsDustSensor.h"
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2 
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float temp;
float humidity;

int rxPin = D7;
int txPin = D8;
SdsDustSensor sds(rxPin, txPin);
float pm10;
float pm25;

void setup() {
  Serial.begin(9600);
  sds.begin();
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void loop() {
  readTemp();
  readAir();
  updateDisplay();
}

void readAir(){
 PmResult pm = sds.readPm();
  if (pm.isOk()) {
    pm25 = pm.pm25;
    pm10 = pm.pm10;
  }
  delay(500);
}

void readTemp(){
  humidity = dht.readHumidity();
  temp = dht.computeHeatIndex(dht.readTemperature(), humidity, false);  
}


void updateDisplay() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(2, 5);
    display.println("P10:");
    display.setCursor(50, 5);
    display.println(pm10);
    display.setCursor(2, 23);
    display.println("P25:");
    display.setCursor(50, 23);
    display.println(pm25);
    display.setCursor(2, 41);
    display.setTextSize(2);
    display.setCursor(4, 45);
    display.print(temp, 0);
    display.cp437(true);
    display.write(248);
    display.print("C");
    display.setCursor(68, 45);
    display.print(humidity, 0);
    display.print("%");
    display.drawRect(0, 40, 128, 24, 1);
    display.drawLine(64, 40, 64, 64, 1);
    display.display();
}

