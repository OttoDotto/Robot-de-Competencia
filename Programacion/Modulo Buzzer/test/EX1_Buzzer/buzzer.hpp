#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
  private:
    int pin;
    int channel;

  public:
    // Constructor
    Buzzer(int buzzerPin, int pwmChannel = 0) {
      pin = buzzerPin;
      channel = pwmChannel;
    }

    // Inicializa el buzzer (PWM)
    void begin(unsigned int freq = 2000, uint8_t resolution = 8) {
      ledcSetup(channel, freq, resolution);
      ledcAttachPin(pin, channel);
    }

    // Toca una nota (frecuencia en Hz)
    void play(int freq) {
      ledcWriteTone(channel, freq);
    }

    // Detiene el sonido
    void stop() {
      ledcWriteTone(channel, 0);
    }

    void playMelody(const int *melody, const int *durations, int length, int baseTime = 500) {
      for (int i = 0; i < length; i++) {
        int noteDuration = baseTime * 4 / durations[i];
        play(melody[i]);
        delay(noteDuration);
        stop();
        delay(noteDuration * 0.2);
      }
    }
};

#endif
