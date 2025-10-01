#include <Arduino.h>

// Pines
const int LED_PIN = 2;        // LED en GPIO2
const int BUTTON_PIN = 4;     // Pulsador en GPIO4 con resistencia externa pull-down

// Variables de control
volatile bool ledState = false;

// ISR
void IRAM_ATTR handleInterrupt() {
  ledState = !ledState;   // Toggle del estado
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);   // IMPORTANTE: sin INPUT_PULLUP, porque usamos pull-down externo

  // Interrupción en flanco de subida (LOW -> HIGH)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
}

void loop() {
  digitalWrite(LED_PIN, ledState);
}
