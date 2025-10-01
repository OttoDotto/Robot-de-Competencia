#ifndef INTERRUPCIONES_ROBOT_H
#define INTERRUPCIONES_ROBOT_H

#include <Arduino.h>

enum RobotMode { MODE_STOPPED, MODE_RUNNING, MODE_CALIBRATION };

class InterrupcionesRobot {
public:
    InterrupcionesRobot(int ledInternalPin, int ledExternalPin,
                        int btnRunPin, int btnStopPin);

    void begin();
    void update(); // Llamar en loop()

    // Asignar funciones externas para cada modo
    void onCalibration(void (*func)());
    void onRunning(void (*func)());
    void onStopped(void (*func)());

private:
    int ledInternal;
    int ledExternal;
    int btnRun;
    int btnStop;

    volatile bool runPressed;
    volatile bool stopPressed;
    volatile RobotMode currentMode;

//    unsigned long calibStartTime;
//    const unsigned long calibDuration = 500;
//    unsigned long lastBlinkTime;
//    const unsigned long blinkInterval = 300;

    void (*funcCalibration)();
    void (*funcRunning)();
    void (*funcStopped)();

    static InterrupcionesRobot* instance;
    static void IRAM_ATTR handleRunStatic();
    static void IRAM_ATTR handleStopStatic();
    void handleRun();
    void handleStop();
};

#endif
