#include "Buzzer.h"
#include "Themes.h"

const int buzzerPin = 17;
Buzzer buzzer(buzzerPin);

void setup() {
  buzzer.begin();
  buzzer.playTheme(imperialMelody, imperialDurations, imperialLength);
}

void loop() {
  // vacío
}
