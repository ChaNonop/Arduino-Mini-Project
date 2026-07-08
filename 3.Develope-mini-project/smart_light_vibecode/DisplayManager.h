#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include "ProjectPins.h"

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void setNumberToShow(int number);
  void updateDisplay(); // Multiplexing happens here

private:
  int numberToDisplay;
  byte digits[NUM_DIGITS];
  byte currentDigitIndex;
  unsigned long lastDigitUpdateTime;
  
  void breakNumberIntoDigits();

  // ตารางรหัสสำหรับเลข 0-9 (Common Cathode)
  const byte digitPatterns[10] = {
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111  // 9
  };
};

#endif
