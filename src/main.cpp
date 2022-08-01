#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Badge2020_Buzzer.h>
#include <Badge2020_TFT.h>

//buzzer
Badge2020_Buzzer buzzer;
float notes[ 13 ] = { 659.25,587.33,369.99,415.3 ,554.37,493.88,293.66,329.63,493.88,440,277.18,329.63,440 };
float lengths[ 13 ] = { 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 6 };
void buzzer_once() {
  buzzer.setVolume( 255 );
  for( int i = 0; i < 13; i++ ) {
    buzzer.setFrequency( notes[i] );
    delay( 120 * lengths[i] );
  }
  buzzer.setVolume(0);
}



//blink non-blocking
const int ledPin = 25;
int ledState = LOW;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
void blink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}


//tft
Badge2020_TFT tft;
//put values on screen
void updateTFT() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 10);
  tft.print("millis: " + String(millis()));
  tft.setCursor(10, 30);
  if (touchRead(27) > 30){
    tft.setTextColor(ST77XX_RED);
  } else {
    tft.setTextColor(ST77XX_GREEN);
  }
  tft.print("Touch 0: " + String(touchRead(27)));
  tft.setCursor(10, 50);
  if (touchRead(14) > 30){
    tft.setTextColor(ST77XX_RED);
  } else {
    tft.setTextColor(ST77XX_GREEN);
  }
  tft.print("Touch 1: " + String(touchRead(14)));
  tft.setCursor(10, 70);
  if (touchRead(13) > 30){
    tft.setTextColor(ST77XX_RED);
  } else {
    tft.setTextColor(ST77XX_GREEN);
  }
  tft.print("Touch 2: " + String(touchRead(13)));
}


void setup() {
  pinMode(ledPin, OUTPUT);
  buzzer_once();
  tft.init(240, 240);
  tft.setRotation( 2 );
}



void loop() {
  //heartbeat
  blink();
  updateTFT();
  delay(20);
}