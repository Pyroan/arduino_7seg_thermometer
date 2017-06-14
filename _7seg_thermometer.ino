#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <stdint.h>

// Bitmasks to write symbols to the
// 7segment display
uint8_t deg = 0x63; // Degree Symbol
uint8_t cel = 0x39; // C
uint8_t fah = 0x71; // F

// Whether or not to display in fahrenheit
bool fahrenheit = false;

// how many readings should be taken from the temp sensor
// every second
int reads_per_sec = 500;

Adafruit_7segment mat = Adafruit_7segment();

// input pin for temperature sensor.
int thermpin = A0;
// toggle button to switch between F and C
int buttonpin = 7;

void setup() {
  pinMode(buttonpin, INPUT);
  mat.begin(0x70);
  mat.print(0xf00d, HEX);
  mat.writeDisplay();
}

void loop() {
  // read the temp
  float avg_temp = 0;
  for (int i = 0; i < reads_per_sec; i++)
  {
    float voltage = (analogRead(thermpin)/1024.0)*5.0;
    float temperature = (voltage - .5) * 100;
    avg_temp += temperature;
    delay(1000/reads_per_sec);
  }
  // check if we need to switch modes
  // promise it's here for a reason.
  if (digitalRead(buttonpin) == HIGH)
  {
    fahrenheit = fahrenheit ? false : true;
  }
  avg_temp /= reads_per_sec;
  if (fahrenheit) {
    avg_temp = (avg_temp * 1.8) + 32;
  }
  // display temperature
  int temp = round(avg_temp);
  mat.writeDigitNum(0,temp/10);
  mat.writeDigitNum(1,temp%10);
  mat.writeDigitRaw(3,deg);
  mat.writeDigitRaw(4,fahrenheit?fah:cel);
  mat.writeDisplay();
}
