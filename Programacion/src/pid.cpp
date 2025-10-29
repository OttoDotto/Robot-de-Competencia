#include <Arduino.h>
#include "config.hpp"
#include "drv8833.hpp"
#include "pid.hpp"

// ============================
// FUNCION CALCULO DE PID
// ============================
float calculo_pid(uint16_t pos, uint32_t now) {
    // Calcular deltaTime
    float  deltaTime = (now - lastTime) / TIME_DIVISOR; // Convertir a segundos
    deb(Serial.printf("deltaTime=%.6f\n", deltaTime);)

    // Calcular el error
    float  error = pos - setpoint;               // valores entre -3500 y 3500
    deb(Serial.printf("error=%.6f\n", error);)

    // Calcular derivativo (tasa de cambio del error)
    float  derivativo = (error - lastError) / deltaTime;

    // Actualizar el último error proporcional
    lastError = error;

    // Calcular integral (acumulación del error)
    integral += error * deltaTime;

    // Calcular la salida del PID
    float  output = (error * Kp) + (derivativo * Kd) + (integral * Ki);

    deb(Serial.printf("PID=%.6f\n", output);)
    return output;
}