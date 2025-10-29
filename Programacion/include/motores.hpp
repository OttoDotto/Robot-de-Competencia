#pragma once
#include <Arduino.h>
#include "config.hpp"
#include "drv8833.hpp"
#include "sensores.hpp"
#include "pid.hpp"

extern int32_t motorSpeedIzq;
extern int32_t motorSpeedDer;
extern Drv8833 motorIzq;
extern Drv8833 motorDer;
extern const uint8_t maxSpeed;

void controlMotores(float correcion);
void moverMotores(int32_t motorSpeedIzq, int32_t motorSpeedDer);