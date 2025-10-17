#include <Arduino.h>

// LEDs
const int LED_INTERNAL = 2;  // LED CALIBRACIÓN
const int LED_EXTERNAL = 13; // LED CORREDOR

// Botones
const int BTN_RUN = 22;  // modo CORREDOR
const int BTN_STOP = 19; // modo CALIBRACIÓN

// Variables
volatile bool runPressed = false;
volatile bool stopPressed = false;

enum RobotMode { MODE_STOPPED, MODE_RUNNING, MODE_CALIBRATION };
volatile RobotMode currentMode = MODE_STOPPED;

// Temporizadores
unsigned long calibStartTime = 0;
const unsigned long calibDuration = 5000; // 5 segundos
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 300;

// ISR del botón RUN
void IRAM_ATTR handleRun() { runPressed = true; }

// ISR del botón STOP
void IRAM_ATTR handleStop() { stopPressed = true; }

void setup() {
  pinMode(LED_INTERNAL, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);

  pinMode(BTN_RUN, INPUT);
  pinMode(BTN_STOP, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

  digitalWrite(LED_INTERNAL, LOW);
  digitalWrite(LED_EXTERNAL, LOW);

  Serial.begin(115200);
  Serial.println("Sistema iniciado. MODO: PARADO");
}

void loop() {
  unsigned long now = millis();

  // --- CALIBRACIÓN ---
  if (stopPressed) {
    digitalWrite(LED_EXTERNAL, LOW);
    currentMode = MODE_CALIBRATION;
    calibStartTime = now;
    stopPressed = false;
    Serial.println("MODO: CALIBRACION");
  }

  if (currentMode == MODE_CALIBRATION) {
    // Parpadeo continuo LED interno
    if (now - lastBlinkTime >= blinkInterval) {
      digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL));
      lastBlinkTime = now;
    }

    // Fin de calibración
    if (now - calibStartTime >= calibDuration) {
      currentMode = MODE_STOPPED;
      digitalWrite(LED_INTERNAL, HIGH);  // queda encendido al terminar
      Serial.println("Calibración completa. MODO: PARADO");
    }
  }

  // --- CORREDOR ---
  if (runPressed) {
    currentMode = MODE_RUNNING;
    digitalWrite(LED_EXTERNAL, HIGH);
    digitalWrite(LED_INTERNAL, LOW); // apagamos calibración al correr
    Serial.println("MODO: CORREDOR");
    runPressed = false;
  }

  // --- PARADO ---
  if (currentMode == MODE_STOPPED) {
    digitalWrite(LED_EXTERNAL, LOW);
  }
}
