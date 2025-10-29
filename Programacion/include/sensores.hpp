#pragma once
#include <Arduino.h>
#include "QTRSensors.h"
#include "buzzer.hpp"

enum Linea { BLANCA, NEGRA };

extern QTRSensors qtr;
extern const uint8_t SensorCount;
extern uint16_t sensorValues[];   // tamaño definido en el .cpp donde se defina
extern uint16_t position;

extern const uint8_t ledCalibracion;
extern Buzzer buzzer;

// Prototipos de funciones
void setupSensores(const uint8_t* sensorPins, uint8_t SensorCount);
void calibrarSensores();
uint16_t leerLinea();
