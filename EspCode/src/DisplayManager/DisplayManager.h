#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <Arduino.h>
#include <FastLED.h>
#include "commonTypes.h"

class DisplayManager {
public:
    DisplayManager(int numLeds, int maxBrightness);
    void initDisplay();
    void displayTime(int hours, int minutes, bool fade);
    void displayTimeContinuous(int hours, int minutes);
    void displayString(String text, bool scndRow = false, bool fade = true, bool display = true, struct CRGB color = CRGB::Red);
    void displayDate(int datetime[3]);
    void setMaxBrightness(int brightness) { maxBrightness = brightness;  };
    void blinkSavedSettings();
    void scrollTextFull(String text, CRGB color);
    void displayNigntTime(int hours, int minutes);
    
    // Funzioni per il fade
    void fadeOut();
    void fadeIn();
    
    // Funzioni per l'effetto neve
    void updateSnow();
    void applySnowOverlay();
    void enableSnow(bool enable) { snowEnabled = enable; }
    void saveBaseDisplay();  // Salva lo stato attuale del display
    
    // Funzioni per il gioco Snake
    void drawSnakeGame(Position* snake, int snakeLength, Position food);
    void clearDisplay();
    
    // Membri pubblici (necessari per accesso esterno)
    CRGB* leds;
    const byte ledMap[256];
    int maxBrightness;
    
private:
    int numLeds;
    const uint8_t dot[7];
    
    // Variabili per l'effetto neve
    bool snowEnabled;
    static const int MATRIX_WIDTH = 16;
    static const int MATRIX_HEIGHT = 16;
    static const int MAX_SNOWFLAKES = 8;
    struct Snowflake {
        int x;
        int y;
        bool active;
    };
    Snowflake snowflakes[MAX_SNOWFLAKES];
    unsigned long lastSnowUpdate;
    CRGB* baseDisplay;  // Buffer per salvare il display senza neve
    
    const byte digits[10][7];
    const byte letters[37][7];
    const bool ok[16][16];
};

#endif // DISPLAYMANAGER_H