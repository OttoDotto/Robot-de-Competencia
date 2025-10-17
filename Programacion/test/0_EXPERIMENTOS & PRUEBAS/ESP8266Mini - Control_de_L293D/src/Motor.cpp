#include "Motor.h"

Motor::Motor(uint8_t pinEnable, uint8_t pinA, uint8_t pinB)
    : _pinEnable(pinEnable), _pinA(pinA), _pinB(pinB)
{
    pinMode(_pinEnable, OUTPUT);
    pinMode(_pinA, OUTPUT);
    pinMode(_pinB, OUTPUT);
    analogWriteRange(1023); // PWM de 10 bits
}

void Motor::setVelocidad(int velocidad) {
    velocidad = constrain(velocidad, -1023, 1023);

    if (velocidad > 0) {
        digitalWrite(_pinA, HIGH);
        digitalWrite(_pinB, LOW);
        analogWrite(_pinEnable, velocidad);
    } else if (velocidad < 0) {
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, HIGH);
        analogWrite(_pinEnable, -velocidad);
    } else {
        digitalWrite(_pinA, LOW);
        digitalWrite(_pinB, LOW);
        analogWrite(_pinEnable, 0);
    }
}
