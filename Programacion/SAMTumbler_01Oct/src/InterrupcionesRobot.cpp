#include "InterrupcionesRobot.hpp"

InterrupcionesRobot* InterrupcionesRobot::instance = nullptr;

InterrupcionesRobot::InterrupcionesRobot(int ledInternalPin, int ledExternalPin,
                                         int btnRunPin, int btnStopPin)
    : ledInternal(ledInternalPin), ledExternal(ledExternalPin),
      btnRun(btnRunPin), btnStop(btnStopPin),
      runPressed(false), stopPressed(false),
      currentMode(MODE_STOPPED),// lastBlinkTime(0), calibStartTime(0),
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

//ANTES: void InterrupcionesRobot::handleStop() { stopPressed = true; }
void InterrupcionesRobot::handleStop() {
    static unsigned long lastInterruptTime = 0;
    unsigned long now = millis();

    if (now - lastInterruptTime > 200) { // 200 ms de debounce
        stopPressed = true;
        lastInterruptTime = now;
    }
}

void InterrupcionesRobot::update() {
    unsigned long now = millis();

    // --- CALIBRACIÓN ---
    if (stopPressed) {
        digitalWrite(ledInternal, HIGH);
        digitalWrite(ledExternal, LOW);
        stopPressed = false;

        // currentMode = MODE_CALIBRATION;
        Serial.println("MODO: CALIBRACION");

        // Ejecuta la calibración directamente una sola vez
        if (funcCalibration) funcCalibration();

        // Termina calibración → vuelve a parado
        currentMode = MODE_STOPPED;
        digitalWrite(ledInternal, LOW);
        Serial.println("Calibración completa. MODO: PARADO");
        return;
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
