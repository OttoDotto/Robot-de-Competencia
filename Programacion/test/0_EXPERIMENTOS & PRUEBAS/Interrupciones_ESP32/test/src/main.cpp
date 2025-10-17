#include <Arduino.h>
#include "InterrupcionesRobot.hpp"

const int LED_INTERNAL = 2;
const int LED_EXTERNAL = 13;
const int BTN_RUN = 22;
const int BTN_STOP = 19;

InterrupcionesRobot robot(LED_INTERNAL, LED_EXTERNAL, BTN_RUN, BTN_STOP);

void calibracion() {
    Serial.println("[Calibración] sensores calibrando...");
}

void correr() {
    Serial.println("[Corredor] motores corriendo...");
}

void parar() {
    Serial.println("[Parado] motores apagados...");
}

void setup() {
    Serial.begin(115200);
    robot.begin();
    robot.onCalibration(calibracion);
    robot.onRunning(correr);
    robot.onStopped(parar);
}

void loop() {
    robot.update();
}
