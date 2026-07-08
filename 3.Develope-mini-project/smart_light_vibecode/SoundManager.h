#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <Arduino.h>
#include "ProjectPins.h"

class SoundManager {
public:
  SoundManager();
  void begin();
  void update();
  bool clapEventTriggered(); // เช็คว่ามีการตบมือครบ 2 ครั้งหรือไม่
  void playReadySound();

private:
  // Clap detection variables
  byte clapCount;
  unsigned long lastClapTime;
  bool lastSoundState;
  
  bool clapEvent; // Flag to signal a double-clap event
};

#endif
