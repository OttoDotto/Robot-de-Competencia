#include <Arduino.h>

// LEDs
const int LED_INTERNAL = 2;
const int LED_EXTERNAL = 13;

// Botones
const int BTN_ON  = 19;   // enciende LED
const int BTN_OFF = 22;  // apaga LED

// ISR: botón ON
void IRAM_ATTR handleOn() {
  digitalWrite(LED_EXTERNAL, HIGH);
}

// ISR: botón OFF
void IRAM_ATTR handleOff() {
  digitalWrite(LED_EXTERNAL, LOW);
}

void setup() {
  pinMode(LED_EXTERNAL, OUTPUT);
  pinMode(BTN_ON, INPUT);
  pinMode(BTN_OFF, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_ON), handleOn, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN_OFF), handleOff, RISING);

  Serial.begin(115200);
  Serial.println("Sistema iniciado. Usa BTN_ON para encender y BTN_OFF para apagar.");
}

void loop() {
  // No hace nada en el loop, todo es por interrupciones
}
