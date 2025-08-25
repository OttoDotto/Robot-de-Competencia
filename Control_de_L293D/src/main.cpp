#include <Arduino.h>
#include "Potenciometro.h"
#include "Motor.h"

Potenciometro pote(A0);
Motor motor(D2, D5, D6);  // ENABLE1=D2, IN1=D5, IN2=D6

void setup() {
    Serial.begin(115200);
    delay(1000);
}

void loop() {
    int raw = pote.leerRaw();  // 0 a 1023
    int velocidad = map(raw, 0, 1023, -1023, 1023);  // sentido y velocidad

    motor.setVelocidad(velocidad);

    Serial.printf("Potenciómetro: %d\tVelocidad PWM: %d\n", raw, velocidad);
    delay(100);
}
