#include "Buzzer.h"
#include "Themes.h"

const int buzzerPin = 17;
Buzzer buzzer(buzzerPin);

void playTheme(const int *melody, const int *durations, int length) {
  int baseTime = 500; // negra = 500 ms
  for (int i = 0; i < length; i++) {
    int noteDuration = baseTime * 4 / durations[i];
    buzzer.play(melody[i]);
    delay(noteDuration);
    buzzer.stop();
    delay(noteDuration * 0.2); // pequeña pausa
  }
}

void setup() {
  buzzer.begin();
  playTheme(imperialMelody, imperialDurations, imperialLength);

//  playTheme(melody, durations, length);

}

void loop() {
  // vacío
}
