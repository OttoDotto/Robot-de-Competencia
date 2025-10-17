#include <Arduino.h>

const int LED_EXTERNAL = 13;  // LED externo
const int BTN_RUN = 22;       // botón RUN
const int BTN_STOP = 19;      // botón STOP

volatile bool buttonRunPressed = false;   // estado RUN
volatile int pressCount = 0;              // contador de presiones

// Variables para parpadeo
bool ledState = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 300; // ms

// Variables de modo
enum RobotMode { MODE_STOPPED, MODE_RUNNING };
RobotMode currentMode = MODE_STOPPED;

bool modePrinted = false; // para no repetir impresión

// ISR para botón RUN
void IRAM_ATTR handleRun() {
  buttonRunPressed = digitalRead(BTN_RUN) == HIGH;
  if (buttonRunPressed) {
    pressCount++;  // incrementa contador al presionar
  }
}

// ISR para botón STOP
void IRAM_ATTR handleStop() {
  pressCount = 0;                
  digitalWrite(LED_EXTERNAL, LOW); 
  ledState = false;
  currentMode = MODE_STOPPED;
  modePrinted = false;
  Serial.println("LED apagado, contador reseteado. MODO: PARADO");
}

void setup() {
  pinMode(LED_EXTERNAL, OUTPUT);
  pinMode(BTN_RUN, INPUT);
  pinMode(BTN_STOP, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

  digitalWrite(LED_EXTERNAL, LOW); 
  Serial.begin(115200);
  Serial.println("Sistema iniciado. Mantener RUN para parpadear, STOP para apagar.");
}

void loop() {
  unsigned long now = millis();

  if (buttonRunPressed && pressCount > 0) {
    // Parpadeo mientras se mantiene RUN
    if (now - lastBlinkTime >= blinkInterval) {
      ledState = !ledState;
      digitalWrite(LED_EXTERNAL, ledState);
      lastBlinkTime = now;
    }
    // No cambia el modo todavía
    currentMode = MODE_STOPPED;
    modePrinted = false;
  } else {
    if (pressCount > 0) {
      // Al soltar RUN → LED externo fijo
      digitalWrite(LED_EXTERNAL, HIGH);
      ledState = true;

      // Cambia modo a CORREDOR solo aquí
      if (!modePrinted) {
        currentMode = MODE_RUNNING;
        Serial.println("MODO: CORREDOR");
        modePrinted = true;
      }
    } else {
      // Si contador = 0 → PARADO
      if (currentMode != MODE_STOPPED && !buttonRunPressed) {
        currentMode = MODE_STOPPED;
        Serial.println("MODO: PARADO");
        modePrinted = false;
      }
    }
  }
}
