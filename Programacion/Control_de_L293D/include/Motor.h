#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(uint8_t pinEnable, uint8_t pinA, uint8_t pinB);
    void setVelocidad(int velocidad);  // rango: -1023 a 1023

private:
    uint8_t _pinEnable;
    uint8_t _pinA;
    uint8_t _pinB;
};

#endif
