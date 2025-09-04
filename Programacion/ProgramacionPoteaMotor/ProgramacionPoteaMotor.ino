#include <Arduino.h>

// M2
#define PinSleepIzq 23
#define PinDigital1Izq 18
#define PinDigital2Izq 21

// M1
#define PinSleepDer 16 //RX2
#define PinDigital1Der 26
#define PinDigital2Der 25

#define Potenciometer 2

#ifndef POTENCIOMETRO_H
#define POTENCIOMETRO_H
class Potenciometro {
public:
    explicit Potenciometro(uint8_t pin);

    int leerRaw();
    float leerVoltaje(float vref = 3.3);

private:
    uint8_t _pin;
};
#endif

#ifndef MOTOR_H
#define MOTOR_H
class Motor {
public:
    Motor(uint8_t pinEnable, uint8_t pinD1, uint8_t pinD2);
    void setDireccion(int direccion);  // rango: -1023 a 1023

private:
    uint8_t _pinEnable;
    uint8_t _pinD1;
    uint8_t _pinD2;
};
#endif

Potenciometro pote(Potenciometer);
Motor motorizq (PinSleepIzq,PinDigital1Izq,PinDigital2Izq);
Motor motorder (PinSleepDer,PinDigital1Der,PinDigital2Der);

Motor::Motor(uint8_t pinEnable, uint8_t pinD1, uint8_t pinD2) : _pinEnable(pinEnable), _pinD1(pinD1), _pinD2(pinD2) {
    pinMode(_pinEnable, OUTPUT);
    pinMode(_pinD1, OUTPUT);
    pinMode(_pinD2, OUTPUT);
}
     // pinEnable es el sleep, pinD1 es el primer pin digital y pinD2 el segundo

//Función para establecer la direccion de los motores
void Motor::setDireccion (int direccion) {
    direccion = constrain(direccion, -1023, 1023);

    if (direccion > 0) {
        digitalWrite(_pinD1, HIGH);
        digitalWrite(_pinD2, LOW);
        analogWrite(_pinEnable, direccion);
    } else if (direccion < 0) {
        digitalWrite(_pinD1, LOW);
        digitalWrite(_pinD2, HIGH);
        analogWrite(_pinEnable, -direccion);
    } else {
        digitalWrite(_pinD1, LOW);
        digitalWrite(_pinD2, LOW);
        analogWrite(_pinEnable, 0);
    }
}

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


void setup() {
    Serial.begin(9600);
    delay(1000);
}

void loop() {
    int rango = pote.leerRaw();  // va de 0 a 1023
    int direccion = map(rango, 0, 1023, -1023, 1023);  // sentido y velocidad

    motorizq.setDireccion(direccion);
    motorder.setDireccion(-direccion);

    Serial.print(rango);
    Serial.println(direccion);
    delay(100);
}




