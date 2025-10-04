// main.cpp

#include <Arduino.h>

#include "InterrupcionesRobot.hpp"

const int LED_INTERNAL = 2;
const int LED_EXTERNAL = 13;
const int BTN_RUN = 22;
const int BTN_STOP = 19;

InterrupcionesRobot robot(LED_INTERNAL, LED_EXTERNAL, BTN_RUN, BTN_STOP);

// Funciones para cada modo
void modoCalibracion() {
    // Código que quieras ejecutar durante cada paso de calibración
    Serial.println("[Calibración] Ejecutando calibración...");
}

void modoParado() {
    // Código que quieras ejecutar mientras está detenido
    Serial.println("[Parado] Motor apagado, esperando comando...");
}

void modoCorredor() {
    // Código que quieras ejecutar cuando está en modo corredor
    Serial.println("[Corredor] Motores encendidos, corriendo...");
}

void setup() {
    robot.begin();
    robot.onCalibration(modoCalibracion);
    robot.onStopped(modoParado);
    robot.onRunning(modoCorredor);
}

void loop() {
    robot.update(); // maneja interrupciones y llama las funciones según el modo
}
