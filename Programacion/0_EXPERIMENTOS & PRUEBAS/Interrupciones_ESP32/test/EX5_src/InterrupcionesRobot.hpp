// InterrupcionesRobot.hpp

#ifndef INTERRUPCIONES_ROBOT_H
#define INTERRUPCIONES_ROBOT_H

#include <Arduino.h>

enum RobotMode { MODE_CALIBRATION, MODE_STOPPED, MODE_RUNNING };

class InterrupcionesRobot {
public:
    InterrupcionesRobot(int ledInternalPin, int ledExternalPin, int btnRunPin, int btnStopPin);

    void begin();
    void update(); // llamar en loop

    // Asignar funciones externas para cada modo
    void onCalibration(void (*func)());
    void onStopped(void (*func)());
    void onRunning(void (*func)());

private:
    int ledInternal;
    int ledExternal;
    int btnRun;
    int btnStop;

    volatile bool buttonRunPressed;
    volatile int pressCount;

    bool ledExternalState;
    unsigned long lastBlinkTime;
    const unsigned long blinkInterval = 300;

    bool modePrinted;

    // Calibración
    int calibStep;
    unsigned long lastCalibTime;
    const unsigned long calibInterval = 1000;
    bool calibActive;

    RobotMode currentMode;

    // Funciones callback externas
    void (*funcCalibration)();
    void (*funcStopped)();
    void (*funcRunning)();

    // ISRs
    static void IRAM_ATTR handleRunStatic();
    static void IRAM_ATTR handleStopStatic();
    void handleRun();
    void handleStop();

    static InterrupcionesRobot* instance; // para acceso en ISRs
};

#endif
