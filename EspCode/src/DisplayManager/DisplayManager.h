#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h>
#include <FastLED.h>

class DisplayManager {
public:
    DisplayManager(int numLeds, int maxBrightness);
    void initDisplay();
    void displayTime(int hours, int minutes, bool fade);
    void displayString(String text, bool scndRow = false, bool fade = true, bool display = true, struct CRGB color = CRGB::Red);
    void displayDate(int datetime[3]);

public:
    int maxBrightness;
private:
    int numLeds;
    
    CRGB* leds;
    const byte digits[10][7];
    const byte letters[26][7];
    const byte ledMap[256];
};

#endif // DISPLAYMANAGER_H