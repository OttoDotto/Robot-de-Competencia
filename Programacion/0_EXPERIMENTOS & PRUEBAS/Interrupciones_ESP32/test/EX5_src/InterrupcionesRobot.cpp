// InterrupcionesRobot.hpp

#include "InterrupcionesRobot.hpp"

InterrupcionesRobot* InterrupcionesRobot::instance = nullptr;

InterrupcionesRobot::InterrupcionesRobot(int ledInternalPin, int ledExternalPin, int btnRunPin, int btnStopPin)
    : ledInternal(ledInternalPin), ledExternal(ledExternalPin),
      btnRun(btnRunPin), btnStop(btnStopPin),
      buttonRunPressed(false), pressCount(0),
      ledExternalState(false), lastBlinkTime(0),
      modePrinted(false), calibStep(0), lastCalibTime(0),
      calibActive(true), currentMode(MODE_CALIBRATION),
      funcCalibration(nullptr), funcStopped(nullptr), funcRunning(nullptr)
{
    instance = this;
}

void InterrupcionesRobot::begin() {
    pinMode(ledInternal, OUTPUT);
    pinMode(ledExternal, OUTPUT);
    pinMode(btnRun, INPUT);
    pinMode(btnStop, INPUT);

    attachInterrupt(digitalPinToInterrupt(btnRun), handleRunStatic, CHANGE);
    attachInterrupt(digitalPinToInterrupt(btnStop), handleStopStatic, RISING);

    digitalWrite(ledInternal, LOW);
    digitalWrite(ledExternal, LOW);

    lastCalibTime = millis();
    Serial.begin(115200);
    Serial.println("Sistema iniciado. MODO CALIBRACION");
}

// Funciones callback
void InterrupcionesRobot::onCalibration(void (*func)()) { funcCalibration = func; }
void InterrupcionesRobot::onStopped(void (*func)())     { funcStopped = func; }
void InterrupcionesRobot::onRunning(void (*func)())     { funcRunning = func; }

// ISRs estáticas
void IRAM_ATTR InterrupcionesRobot::handleRunStatic() { if(instance) instance->handleRun(); }
void IRAM_ATTR InterrupcionesRobot::handleStopStatic() { if(instance) instance->handleStop(); }

// Manejo ISR
void InterrupcionesRobot::handleRun() {
    buttonRunPressed = digitalRead(btnRun) == HIGH;
    if(buttonRunPressed) pressCount++;
}

void InterrupcionesRobot::handleStop() {
    pressCount = 0;
    digitalWrite(ledExternal, LOW);
    if(currentMode != MODE_STOPPED) digitalWrite(ledInternal, LOW); // pierde calibración
    ledExternalState = false;
    currentMode = MODE_STOPPED;
    modePrinted = false;
}

// Update principal: se llama en loop()
void InterrupcionesRobot::update() {
    unsigned long now = millis();

    // --- CALIBRACION ---
    if (currentMode == MODE_CALIBRATION && calibActive) {
        // Parpadeo continuo
        static unsigned long lastBlinkTime = 0;
        if (now - lastBlinkTime >= 300) {
            digitalWrite(ledInternal, !digitalRead(ledInternal));
            lastBlinkTime = now;
        }

        // Ejecutar calibración en cada ciclo
        if (funcCalibration) funcCalibration();

        // Condición de salida (ejemplo: después de 5 segundos)
        if (now - lastCalibTime >= 5000) {
            Serial.println("Calibración completa");
            digitalWrite(ledInternal, HIGH);
            calibActive = false;
            currentMode = MODE_STOPPED;
            modePrinted = false;
        }
        return;
    }

    // --- RUN / PARADO ---
    if (buttonRunPressed && pressCount > 0) {
        // Mientras se mantiene RUN → parpadeo
        if (now - lastBlinkTime >= blinkInterval) {
            ledExternalState = !ledExternalState;
            digitalWrite(ledExternal, ledExternalState);
            lastBlinkTime = now;
        }
        // OJO: acá NO cambiamos a STOPPED todavía.    
    }
    else if (pressCount > 0) {
            // Al soltar → LED fijo = modo RUNNING
            digitalWrite(ledExternal, HIGH);
            ledExternalState = true;

            if (currentMode != MODE_RUNNING) {
                currentMode = MODE_RUNNING;
                if (!modePrinted) {
                    Serial.println("MODO: CORREDOR");
                    modePrinted = true;
                }
            }
            // Ejecutar CORRER en cada ciclo
            if (funcRunning) funcRunning();
    }

    if (currentMode == MODE_STOPPED) {
        if (!modePrinted) {
            // Serial.println("MODO: PARADO");
            modePrinted = true;
        }
        // Ejecutar PARAR en cada ciclo
        if (funcStopped) funcStopped();
    }
}
