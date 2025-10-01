#include <Arduino.h>

#include "Buzzer.hpp"
#include "Pitches.hpp"

Buzzer buzzer(17);

void setup() {
    Serial.begin(115200);
    buzzer.begin();

    // Reproducir todas las notas fundamentales de la octava 4 y 5
    for (int octave = 4; octave <= 5; octave++) {
        for (int n = 0; n < 7; n++) {
            int freq = getNote(n, octave);

            Serial.print(noteNames[n]);
            Serial.print(octave);
            Serial.print(": ");
            Serial.println(freq);
            
            buzzer.play(freq);
            
            delay(400);
            buzzer.stop();
            delay(100);
        }
    }
}

void loop() {}
