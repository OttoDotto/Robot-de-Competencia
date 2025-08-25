#include "Potenciometro.h"

Potenciometro::Potenciometro(uint8_t pin) : _pin(pin) {
    pinMode(_pin, INPUT);
}

int Potenciometro::leerRaw() {
    return analogRead(_pin);
}

float Potenciometro::leerVoltaje(float vref) {
    int raw = leerRaw();
    return (raw / 1023.0f) * vref;
}
