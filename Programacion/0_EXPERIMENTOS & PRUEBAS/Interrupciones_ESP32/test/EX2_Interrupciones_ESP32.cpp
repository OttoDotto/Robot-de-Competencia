#include <Arduino.h>

// LEDs
const int LED_INTERNAL = 2;    // LED interno
const int LED_EXTERNAL = 13;

// Botones
const int BTN_RUN  = 22;   // enciende LED
const int BTN_STOP = 19;  // apaga LED

// Estados del robot
enum RobotMode {
  MODE_CALIBRATION,
  MODE_STOPPED,
  MODE_RUNNING
};

volatile RobotMode currentMode = MODE_CALIBRATION;
volatile bool modeChanged = true;

// Variables para calibración no bloqueante
int calibStep = 0;
unsigned long lastCalibTime = 0;
bool calibActive = false;

// ISR: botón RUN
void IRAM_ATTR handleRun() {
  currentMode = MODE_RUNNING;
  modeChanged = true;
}

// ISR: botón STOP
void IRAM_ATTR handleStop() {
  currentMode = MODE_STOPPED;
  modeChanged = true;
}

void setup() {
  pinMode(LED_INTERNAL, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);

  pinMode(BTN_RUN, INPUT);
  pinMode(BTN_STOP, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

  Serial.begin(115200);
  Serial.println("Sistema iniciado en modo CALIBRACION");

  // Inicializa calibración
  calibStep = 0;
  calibActive = true;
  lastCalibTime = millis();
}

void loop() {
  if (modeChanged) {
    modeChanged = false;

    switch (currentMode) {
      case MODE_STOPPED:
        digitalWrite(LED_INTERNAL, LOW);
        digitalWrite(LED_EXTERNAL, LOW);
        calibActive = false;
        Serial.println("Modo: PARADO");
        break;

      case MODE_RUNNING:
        digitalWrite(LED_EXTERNAL, HIGH);
        Serial.println("Modo: CORREDOR");
        break;

      case MODE_CALIBRATION:
        digitalWrite(LED_EXTERNAL, LOW);
        calibStep = 0;
        calibActive = true;
        lastCalibTime = millis();
        Serial.println("Modo: CALIBRACION");
        break;
    }
  }

  // --- Calibración no bloqueante ---
  if (calibActive && currentMode == MODE_CALIBRATION) {
    unsigned long now = millis();
    if (now - lastCalibTime >= 1000) {  // cada segundo
      calibStep++;
      lastCalibTime = now;

      if (calibStep <= 3) {
        Serial.println(calibStep);
        digitalWrite(LED_INTERNAL, !digitalRead(LED_INTERNAL)); // parpadeo
      }

      if (calibStep > 3) {
        Serial.println("Calibrado");
        digitalWrite(LED_INTERNAL, HIGH); // listo
        calibActive = false;
        currentMode = MODE_STOPPED;  // pasa a parado
        modeChanged = true;
      }
    }
  }
}