#include "DS3231.h" // DS3231 library to control the RTC
#include <Wire.h>
#include "images.h"
#include "displayDefinitions.h"
#include "wifisettings.h"
#include "getTime.h"
#include "OTA.h"

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

void displaydugDug(uint8_t times)
{
  FastLED.clear();

  for (uint8_t j = 0; j < times; j++)
  {

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = pgm_read_dword(&(BombJack01[i])); // Read array from Flash
    }

    FastLED.show();
    delay(500);

    // Put Qbert second frame
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = pgm_read_dword(&(BombJack02[i]));
    }

    FastLED.show();
    delay(500);
  }
}


void setup()
{
  int datetime[8];
  Serial.begin(9600);
  Wire.begin(33, 35);
  FastLED.addLeds<WS2812B, 3, GRB>(leds, 256);
  FastLED.setBrightness(150);
  displayString("CNN" , false, false);
  if (initWIFI()) {
    displayString("OK", false, false);
  }
  delay(1000);  
  OTAInit();
  initgetTime();
  // Get the date and time from the NTP server and set the RTC
  Clock.setEpoch(getDateTime(datetime));
  // Set the time on the RTC
  Clock.setDoW(datetime[7]);  
  displayString("FIN", false, false);
  delay(1000); 
  
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  displayQbert(4);
}


void loop()
{
  DateTime now = myRTC.now();;
  //new array for the time 
  int day = now.day();
  int month = now.month();
  int dayOfWeek = now.dayOfTheWeek();
  int datetime[3] = {day, month,dayOfWeek};
  //Print the time
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" ");
  Serial.print(now.dayOfTheWeek(), DEC);
  Serial.println();

  // displayString("XYZ");
  displayTime(now.hour(), now.minute(), true);
  delay(20000);
  displayDate(datetime);
  delay(10000);
  
  switch (random(1, 3))
  {
    case 1:
      displayQbert(4);
      break;
    case 2:
      displaydugDug(4);
      break;
    case 3:
      rainbow();
      break;
    default:
      break;
  }
  
}
