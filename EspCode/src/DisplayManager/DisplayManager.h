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
    void setMaxBrightness(int brightness) { maxBrightness = brightness;  };
    void blinkSavedSettings();
    void scrollTextFull(String text, CRGB color);
    void displayNigntTime(int hours, int minutes);
public:
    int maxBrightness;
private:
    int numLeds;
    const uint8_t dot[7] ;

    // Funzioni per il fade
    void fadeOut();
    void fadeIn();
    
    CRGB* leds;
    const byte digits[10][7];
    const byte letters[37][7];
    const byte ledMap[256];
    //matrix to save v [16][16]
    const bool ok[16][16];
};

#endif // DISPLAYMANAGER_H