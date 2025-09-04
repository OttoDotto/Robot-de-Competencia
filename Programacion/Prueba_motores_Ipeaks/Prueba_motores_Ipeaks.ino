#include <Arduino.h>

// ------------------ PINES ------------------
// Motor IZQUIERDO
#define PinSleep_M2 23
#define IN1_M2 18
#define IN2_M2 21

// Motor DERECHO
#define PinSleep_M1 16 // RX2
#define IN1_M1 26
#define IN2_M1 25

// ------------------ CLASE DRIVER PUENTE H ------------------
class DRV8833 {
public:
    DRV8833(uint8_t pinSleep, uint8_t pinIN1, uint8_t pinIN2);

    void adelante();
    void atras();
    void freno();
    void sleep();
    void wake();   // nuevo método: volver a activar el driver

private:
    uint8_t _pinSleep;
    uint8_t _pinIN1;
    uint8_t _pinIN2;
};

// constructor de la clase
DRV8833::DRV8833(uint8_t pinSleep, uint8_t pinIN1, uint8_t pinIN2)
: _pinSleep(pinSleep), _pinIN1(pinIN1), _pinIN2(pinIN2) {
    pinMode(_pinSleep, OUTPUT);
    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);

    digitalWrite(_pinSleep, HIGH); // habilitar el driver
    freno();
}

// Métodos de control de dirección
void DRV8833::adelante() {
    digitalWrite(_pinIN1, HIGH);
    digitalWrite(_pinIN2, LOW);
}

void DRV8833::atras() {
    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, HIGH);
}

void DRV8833::freno() {
    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, LOW);
}

void DRV8833::sleep() {
    digitalWrite(_pinSleep, LOW);  // desactiva el driver
}

void DRV8833::wake() {
    digitalWrite(_pinSleep, HIGH); // activa el driver
}

// ------------------ OBJETOS ------------------
DRV8833 MotorIzq(PinSleep_M2, IN1_M2, IN2_M2);
DRV8833 MotorDer(PinSleep_M1, IN1_M1, IN2_M1);

// ------------------ PROGRAMA ------------------
void setup() {
    Serial.begin(115200);
    Serial.println("Prueba direccional de motores");
}

void loop() {
    Serial.println("Adelante");
    MotorIzq.adelante();
    MotorDer.adelante();
    delay(2000);

    Serial.println("Atrás");
    MotorIzq.atras();
    MotorDer.atras();
    delay(2000);

/*
    Serial.println("Sleep");
    MotorIzq.sleep();
    MotorDer.sleep();
    delay(2000);

    Serial.println("Wake + Adelante");
    MotorIzq.wake();
    MotorDer.wake();
    MotorIzq.adelante();
    MotorDer.adelante();
    delay(2000);

    MotorIzq.freno();
    MotorDer.freno();
    delay(2000);*/

}
