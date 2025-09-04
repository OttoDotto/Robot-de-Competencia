#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
  private:
    int pin;

  public:
    Buzzer(int buzzerPin) {
      pin = buzzerPin;
    }

    void begin(unsigned int freq = 2000, uint8_t resolution = 8) {
      // En Core 3.x se usa ledcAttach
      ledcAttach(pin, freq, resolution);
    }

    void play(int freq) {
      ledcWriteTone(pin, freq);
    }

    void stop() {
      ledcWriteTone(pin, 0);
    }
};

#endif
