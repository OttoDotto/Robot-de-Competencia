#pragma once

extern uint16_t setpoint;
extern uint16_t zonaMuerta;

extern float  lastError;   // Error previo         -   control D
extern float  integral;    // Acumulador           -   control I
extern uint32_t lastTime;  // Millis previo        -   delta Tiempo

float calculo_pid(uint16_t pos, uint32_t now);
