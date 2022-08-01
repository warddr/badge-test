#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <Badge2020_Buzzer.h>
#include <Badge2020_TFT.h>
#include <Adafruit_NeoPixel.h>
#include <Badge2020_Accelerometer.h>

#define WS2812_PIN 2
#define WS2812_NUMPIXELS 5

void changeNeoPixels();
void updateTFT();

Adafruit_NeoPixel neoPixels(WS2812_NUMPIXELS, WS2812_PIN, NEO_GRB);

//accelerometer
Badge2020_Accelerometer accelerometer;
int accelerometer_initialised = -1;

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
unsigned long previousMillis = 0;
unsigned int IR_level;
const long interval = 200;
void blink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      pinMode(ledPin, OUTPUT);
      delay(20);
      ledState = HIGH;
    } else {
      ledState = LOW;
      pinMode(ledPin, INPUT);
      delay(100);
      IR_level = analogRead(ledPin);
    }
    digitalWrite(ledPin, ledState);
    changeNeoPixels();
    updateTFT();
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
  tft.setCursor(10,90);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("IR-level: " + String(IR_level));
  tft.setCursor(10,110);
  if (accelerometer_initialised == 0) {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("ACC OK");
    int16_t x, y, z;
    accelerometer.readXYZ(x, y, z);
    accelerometer.mgScale(x, y, z);
    // make it a unit length vector
    double length = sqrt( x*x + y*y + z*z );
    int unitx = round(240-((x / length)+1)*120);
    double unity = round(100-((y / length)+1)*50);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(unitx,130+unity);
    tft.print("0");
  }
  else{
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(3);
    tft.print("ACC NOT OK");    
  }
}


void setup() {
  buzzer_once();
  tft.init(240, 240);
  tft.setRotation( 2 );
  neoPixels.begin();
  Wire.begin();
  accelerometer_initialised = accelerometer.init(LIS2DH12_RANGE_2GA);
}



void loop() {
  //heartbeat
  blink();

}

int currCol = 0;
int colors[4][3] = {{15, 0, 0}, {0, 15, 0}, {0, 0, 15}, {15, 15, 15}};

void changeNeoPixels() {
  currCol = (currCol + 1) % 4;
  for (int i = 0; i < WS2812_NUMPIXELS; i++) { // For each pixel...
    neoPixels.setPixelColor(i, neoPixels.Color(colors[currCol][0], colors[currCol][1], colors[currCol][2]));
    neoPixels.show();   // Send the updated pixel colors to the hardware.
  }
}