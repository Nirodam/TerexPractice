#include <M5Stack.h>
#include "M5StackUpdater.h"
#include "Image.h"
#include "DHT12.h"
#include <Wire.h>                  //The DHT12 uses I2C comunication.
DHT12 dht12(FAHRENHEIT, 0x5c);     //Preset scale "CELSIUS, FAHRENHEIT, or KELVIN" and ID "0x5c".
float tempAdj = 0;

int lcdBrightness = 0;
bool currentButtonA = 0;
bool lastButtonA = 0;
bool currentButtonB = 0;
bool lastButtonB = 0;
bool currentButtonC = 0;
bool lastButtonC = 0;

int16_t xPic = 40;
int16_t yPic = 70;
int16_t wPic = 240;
int16_t hPic = 167;

void setup() {
  pinMode(25, OUTPUT);  //speaker
  digitalWrite(25, LOW);
  //dacWrite(25, 0);  //set speaker to 0 volts A/C
  
  M5.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
  M5.Lcd.setBrightness(100);        // LCS Screen is 320 x 240 in size
  M5.Lcd.drawBitmap(xPic, yPic, wPic, hPic, image);
}

void loop() {
  float tmp = dht12.readTemperature() + tempAdj; //Adjusted Temperature
  float hum = dht12.readHumidity();
  Serial.printf("Temperature: %2.1f F  Humidity: %0.1f%%\r\n", tmp, hum);

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.printf("Temp: %2.1f F \r\nHumid: %2.1f%%  \r\n", tmp, hum);

  currentButtonA = M5.BtnA.read();
  currentButtonB = M5.BtnB.read();
  currentButtonC = M5.BtnC.read();

  if(currentButtonA == 1 && currentButtonA != lastButtonA){
    tempAdj = tempAdj - 0.1;
  }
  lastButtonA = currentButtonA;

  if(currentButtonC == 1 && currentButtonC != lastButtonC){
    tempAdj = tempAdj + 0.1;
  }
  lastButtonC = currentButtonC;
  
  if(currentButtonB == 1 && currentButtonB != lastButtonB){
    lcdBrightness++;
    if(lcdBrightness > 5){
      lcdBrightness = 0;
    }
  }
  lastButtonB = currentButtonB;
  
  switch(lcdBrightness) {
    case 0:
      M5.Lcd.setBrightness(255);
      break;
    case 1:
      M5.Lcd.setBrightness(120);
      break;
    case 2:
      M5.Lcd.setBrightness(60);
      break;
    case 3:
      M5.Lcd.setBrightness(10);
      break;
    case 4:
      M5.Lcd.setBrightness(5);
      break;
    case 5:
      M5.Lcd.setBrightness(0);
      break;
  }
}
