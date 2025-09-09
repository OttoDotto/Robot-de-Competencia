#include <Arduino.h>
#include "buzzer.h"

Buzzer::Buzzer(int buzzerPin) {
    pin = buzzerPin;
}

void Buzzer::begin(unsigned int freq = 2000, uint8_t resolution = 8) {
    // En Core 3.x se usa ledcAttach
    ledcAttach(pin, freq, resolution);
}

void Buzzer::play(int freq) {
    ledcWriteTone(pin, freq);
}

void Buzzer::stop() {
    ledcWriteTone(pin, 0);
}

void Buzzer::playTheme(const int *melody, const int *durations, int length) {
  int baseTime = 500; // negra = 500 ms
  for (int i = 0; i < length; i++) {
    int noteDuration = baseTime * 4 / durations[i];
    buzzer.play(melody[i]);
    delay(noteDuration);
    buzzer.stop();
    delay(noteDuration * 0.2); // pequeña pausa
  }
}
