#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 256
CRGB leds[NUM_LEDS];



const byte digits[10][7] = {
    {B0001111, B1011001, B1011001, B1011001, B1011001, B1011001, B0001111}, // 0
    {B0000110, B0001010, B0000010, B0000010, B0000010, B0000010, B1101111}, // 1
    {B0101111, B0000001, B0000001, B1101111, B1001000, B1001000, B1101111}, // 2
    {B1101111, B0000001, B0000001, B0101111, B0000001, B0000001, B1101111}, // 3
    {B1001001, B1001001, B1001001, B1111111, B0000001, B0000001, B0000001}, // 4
    {B1111111, B1001000, B1001000, B1111111, B0000001, B0000001, B1111111}, // 5
    {B0111111, B1001000, B1001000, B1111111, B1001001, B1001001, B0111111}, // 6
    {B1111111, B0000001, B0000001, B0000001, B0000001, B0000001, B0000001}, // 7
    {B0001111, B1011001, B1011001, B1011111, B1011001, B1011001, B0001111}, // 8
    {B0001111, B1011001, B1011001, B1011111, B1010001, B1010001, B0001111}  // 9
};

const byte letters[26][7] = {
    {B0000100, B0001010, B0010001, B0010001, B0011111, B0010001, B0110001}, // A
    {B0111110, B0010001, B0010001, B1111110, B0010001, B0010001, B0111110}, // B
    {B0001110, B0010001, B0010000, B0010000, B0010000, B0010001, B0001110}, // C
    {B1111110, B0010001, B0010001, B0010001, B0010001, B0010001, B1111110}, // D
    {B1111111, B0010001, B0010000, B0111110, B0010000, B0010001, B1111111}, // E
    {B1111111, B0010001, B0010000, B0111110, B0010000, B0010000, B0010000}, // F
    {B0001110, B0110001, B0110000, B0110111, B0110001, B0110001, B0001110}, // G
    {B0110001, B0110001, B0110001, B0111111, B0110001, B0110001, B0110001}, // H
    {B1101110, B0000100, B0000100, B0000100, B0000100, B0000100, B1101110}, // I
    {B0001111, B0000010, B0000010, B0000010, B0100010, B0110010, B0001100}, // J
    {B0110001, B0110010, B0110100, B0111000, B0110100, B0110010, B0110001}, // K
    {B0010000, B0010000, B0010000, B0010000, B0010000, B0010000, B1101111}, // L
    {B1010001, B1110001, B1011011, B1010101, B1010001, B1010001, B1010001}, // M
    {B1010001, B1110001, B1010001, B1011001, B1010101, B1010011, B1010001}, // N
    {B0001110, B0110001, B0110001, B0110001, B0110001, B0110001, B0001110}, // O
    {B0111110, B0010001, B0010001, B0011110, B0010000, B0010000, B0010000}, // P
    {B0001110, B0110001, B0110001, B0110001, B0110101, B0110010, B0001111}, // Q
    {B0111110, B0010001, B0010001, B0011110, B0011000, B0010100, B0010010}, // R
    {B0001110, B0110001, B0110000, B0001110, B0000001, B0110001, B0001110}, // S
    {B1111111, B0000100, B0000100, B0000100, B0000100, B0000100, B0000100}, // T
    {B0110001, B0110001, B0110001, B0110001, B0110001, B0110001, B0001110}, // U
    {B0110001, B0110001, B0110001, B0110001, B0110001, B0001010, B0000100}, // V
    {B1010001, B1010001, B1010001, B1010001, B1010101, B1011011, B1110001}, // W
    {B1000001, B1000001, B0100010, B0010100, B0100010, B1000001, B1000001}, // X
    {B1000001, B1000001, B0100010, B0010100, B0001000, B0001000, B0001000}, // Y
    {B1111111, B0000001, B0000010, B0000100, B0001000, B0010000, B1111111}, // Z
};



// Mappa la disposizione dei LED nella tua matrice
const uint8_t ledMap[] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160,
    176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
    };

void displayTime(int hours, int minutes)
{
  int digitWidth = 4;  // Larghezza di ogni cifra
  int digitHeight = 7; // Altezza di ogni cifra

  int hourTens = hours / 10;
  int hourOnes = hours % 10;
  int minTens = minutes / 10;
  int minOnes = minutes % 10;

  FastLED.clear(); // Pulisci la matrice di LED

  // Disegna le cifre
  for (int i = 0; i < digitHeight; i++)
  {
    for (int j = 0; j < digitWidth; j++)
    {
      if (digits[hourTens][i] & (1 << (digitWidth - 1 - j)))
      {
        leds[ledMap[i * digitWidth + j + (12 * i)]] = CRGB::Blue; // 12 per w= 4 11 per w=5
      }
      if (digits[hourOnes][i] & (1 << (digitWidth - 1 - j)))
      {
        leds[ledMap[i * digitWidth + j + 5 + (12 * i)]] = CRGB::Blue;
      }
      if (digits[minTens][i] & (1 << (digitWidth - 1 - j)))
      {
        leds[ledMap[i * digitWidth + j + 151 + (12 * i)]] = CRGB::Blue;
      }
      if (digits[minOnes][i] & (1 << (digitWidth - 1 - j)))
      {
        leds[ledMap[i * digitWidth + j + 156 + (12 * i)]] = CRGB::Blue;
      }
    }
  }

  FastLED.show();
};


void displayString(String text)
{
  int letterWidth = 5;  // Larghezza di ogni lettera
  int letterHeight = 7; // Altezza di ogni lettera

  FastLED.clear(); // Pulisci la matrice di LED

  int startX = 0; // Posizione iniziale sulla matrice di LED

  for (int k = 0; k < text.length(); k++)
  {
    char currentChar = text.charAt(k);

    // Converte il carattere in maiuscolo
    if (currentChar >= 'a' && currentChar <= 'z')
    {
      currentChar = currentChar - 'a' + 'A';
    }

    // Verifica se il carattere è una lettera
    if (currentChar >= 'A' && currentChar <= 'Z')
    {
      int letterIndex = currentChar - 'A';
      for (int i = 0; i < letterHeight; i++)
      {
        for (int j = 0; j < letterWidth; j++)
        {
          if (letters[letterIndex][i] & (1 << (letterWidth - 1 - j)))
          {
            leds[ledMap[ startX + i * letterWidth + j+ 144 + (11 * i)]] = CRGB::Red; // Usa il colore che desideri
          }
        }
      }
      // Sposta la posizione per la prossima lettera
      startX += letterWidth + 0; // Aggiungi un pixel di spazio tra le lettere

    }
  }

  FastLED.show();
}