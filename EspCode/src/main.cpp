#include "DS3231.h" // DS3231 library to control the RTC
#include <Wire.h>
#include "images.h"
#include "displayDefinitions.h"

// RTC OBJECT CREATION
RTClib myRTC;
DS3231 Clock;

void rainbow()
{
  for (int j = 0; j < 255; j++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(i - (j * 2), 255, 150); /* The higher the value 4 the less fade there is and vice versa */
    }
    FastLED.show();
    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
}

void setup()
{
  FastLED.addLeds<WS2812B, 3, GRB>(leds, 256);
  FastLED.setBrightness(150);
  Wire.begin(33, 35);
  Serial.begin(9600);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}
int hours = 00;
int minutes = 00;

void displayQbert(uint8_t times)
{
  FastLED.clear();

  for (uint8_t j = 0; j < times; j++)
  {

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = pgm_read_dword(&(Qbert01[i])); // Read array from Flash
    }

    FastLED.show();
    delay(500);

    // Put Qbert second frame
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = pgm_read_dword(&(Qbert02[i]));
    }

    FastLED.show();
    delay(500);
  }
}

void loop()
{
  DateTime now = myRTC.now();
  // displayString("XYZ");
  displayTime(now.hour(), now.minute());
  delay(2000);
  displayQbert(8);
  // rainbow();
}
