#ifndef POTENCIOMETRO_H
#define POTENCIOMETRO_H

#include <Arduino.h>

class Potenciometro {
public:
    explicit Potenciometro(uint8_t pin);

    int leerRaw();
    float leerVoltaje(float vref = 3.3);

private:
    uint8_t _pin;
};

#endif
