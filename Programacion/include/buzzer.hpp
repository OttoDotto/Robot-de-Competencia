#ifndef PITCHES_H
#define PITCHES_H

// Notas fundamentales en octava 4 - GRAVES
const uint16_t NOTE_C4 = 262;    //DO
const uint16_t NOTE_D4 = 294;    //RE
const uint16_t NOTE_E4 = 330;    //MI
const uint16_t NOTE_F4 = 349;    //FA
const uint16_t NOTE_G4 = 392;    //SOL
const uint16_t NOTE_A4 = 440;    //LA
const uint16_t NOTE_B4 = 494;    //SI

// Notas fundamentales en octava 5 - AGUDAS
const uint16_t NOTE_C5 = 523;
const uint16_t NOTE_D5 = 587;
const uint16_t NOTE_E5 = 659;
const uint16_t NOTE_F5 = 698;
const uint16_t NOTE_G5 = 784;
const uint16_t NOTE_A5 = 880;
const uint16_t NOTE_B5 = 988;

#endif


#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
  private:
    uint8_t pin;
    uint8_t channel;
    
  public:
    // Constructor
    Buzzer(uint8_t buzzerPin, uint8_t pwmChannel = 0) {
      pin = buzzerPin;
      channel = pwmChannel;
    }

    // Inicializa el buzzer (PWM)
    void begin(uint16_t freq = 2000, uint8_t resolution = 8) {
      ledcSetup(channel, freq, resolution);
      ledcAttachPin(pin, channel);
    }

    // Toca una nota (frecuencia en Hz)
    void play(uint16_t freq) {
      ledcWriteTone(channel, freq);
    }

    // Detiene el sonido
    void stop() { ledcWriteTone(channel, 0);  }

    void playMelody(const uint16_t *melody, const uint16_t *durations, uint16_t length, uint16_t baseTime = 500) {
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
