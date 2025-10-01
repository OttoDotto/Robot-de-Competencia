#include <Arduino.h>

// Pin donde está conectado el transistor
const int buzzerPin = 17;
// Canal PWM que usaremos en el ESP32
const int buzzerChannel = 0;

void setup() {
  // Configuración del canal PWM
  ledcSetup(buzzerChannel, 2000, 8);     // canal, frecuencia inicial, resolución
  ledcAttachPin(buzzerPin, buzzerChannel);

  // Ejemplo: tocar tres notas
  ledcWriteTone(buzzerChannel, 440);  // A4
  delay(500);

  ledcWriteTone(buzzerChannel, 523);  // C5
  delay(500);

  ledcWriteTone(buzzerChannel, 659);  // E5
  delay(500);

  ledcWriteTone(buzzerChannel, 0);    // silencio
}

void loop() {
  // vacío
}
