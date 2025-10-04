#include "InterrupcionesRobot.hpp"

InterrupcionesRobot* InterrupcionesRobot::instance = nullptr;

InterrupcionesRobot::InterrupcionesRobot(int ledInternalPin, int ledExternalPin,
                                         int btnRunPin, int btnStopPin)
    : ledInternal(ledInternalPin), ledExternal(ledExternalPin),
      btnRun(btnRunPin), btnStop(btnStopPin),
      runPressed(false), stopPressed(false),
      currentMode(MODE_STOPPED),
      calibStartTime(0), lastBlinkTime(0),
      funcCalibration(nullptr), funcRunning(nullptr), funcStopped(nullptr)
{
    instance = this;
}

void InterrupcionesRobot::begin() {
    pinMode(ledInternal, OUTPUT);
    pinMode(ledExternal, OUTPUT);
    pinMode(btnRun, INPUT);
    pinMode(btnStop, INPUT);

    attachInterrupt(digitalPinToInterrupt(btnRun), handleRunStatic, RISING);
    attachInterrupt(digitalPinToInterrupt(btnStop), handleStopStatic, RISING);

    digitalWrite(ledInternal, LOW);
    digitalWrite(ledExternal, LOW);
}

void InterrupcionesRobot::onCalibration(void (*func)()) { funcCalibration = func; }
void InterrupcionesRobot::onRunning(void (*func)()) { funcRunning = func; }
void InterrupcionesRobot::onStopped(void (*func)()) { funcStopped = func; }

void IRAM_ATTR InterrupcionesRobot::handleRunStatic() { if(instance) instance->handleRun(); }
void IRAM_ATTR InterrupcionesRobot::handleStopStatic() { if(instance) instance->handleStop(); }

void InterrupcionesRobot::handleRun() { runPressed = true; }
void InterrupcionesRobot::handleStop() { stopPressed = true; }

void InterrupcionesRobot::update() {
    unsigned long now = millis();

    // --- CALIBRACIÓN ---
    if (stopPressed) {
        digitalWrite(ledExternal, LOW);
        currentMode = MODE_CALIBRATION;
        calibStartTime = now;
        stopPressed = false;
        Serial.println("MODO: CALIBRACION");
    }

    if (currentMode == MODE_CALIBRATION) {
        // Parpadeo LED interno
        if (now - lastBlinkTime >= blinkInterval) {
            digitalWrite(ledInternal, !digitalRead(ledInternal));
            lastBlinkTime = now;
        }

        // Función externa de calibración
        if (funcCalibration) funcCalibration();

        // Termina calibración
        if (now - calibStartTime >= calibDuration) {
            currentMode = MODE_STOPPED;
            digitalWrite(ledInternal, HIGH);
            Serial.println("Calibración completa. MODO: PARADO");
        }
        return; // mientras calibra, no ejecutar RUN
    }

    // --- CORREDOR ---
    if (runPressed) {
        currentMode = MODE_RUNNING;
        digitalWrite(ledExternal, HIGH);
        digitalWrite(ledInternal, LOW);
        Serial.println("MODO: CORREDOR");
        runPressed = false;
    }

    if (currentMode == MODE_RUNNING && funcRunning) funcRunning();

    // --- PARADO ---
    if (currentMode == MODE_STOPPED && funcStopped) funcStopped();
}
