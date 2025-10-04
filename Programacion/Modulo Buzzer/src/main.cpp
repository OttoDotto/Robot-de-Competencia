#include <Arduino.h>

#include "Buzzer.hpp"
#include "Pitches.hpp"

Buzzer buzzer(17);

// Cumpleaños feliz ? (simplificada)
int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_C5 };
int durations[] = { 4, 4, 4, 2 };

void setup() {
  buzzer.begin();
  buzzer.playMelody(melody, durations, 4); // mucho más limpio
}

void loop() {}
