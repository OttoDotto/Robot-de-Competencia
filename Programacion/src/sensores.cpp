#include <Arduino.h>
#include "sensores.hpp"
#include "motores.hpp"
#include "config.hpp"


// ============================
// FUNCION CALIBRAR
// ============================
void calibrarSensores() {
    mute(
        buzzer.play(NOTE_A4);
        delay(150);
        buzzer.stop();
    );

    motorIzq.stop();
    motorDer.stop();

    digitalWrite(ledCalibracion, HIGH);
    deb(Serial.println("Calibrando sensores..."); )

    for (uint16_t i = 0; i < 300; i++) { qtr.calibrate(); } 
    digitalWrite(ledCalibracion, LOW);
    deb(Serial.println("Calibracion lista!");)

    mute(
        buzzer.play(NOTE_C5);
        delay(200);
        buzzer.stop();
    )
}


// ============================
// LECTURA DE POSICIÓN
// ============================
uint16_t leerLinea() {
    if (linea_competencia == BLANCA)
        position = qtr.readLineWhite(sensorValues);
    else
        position = qtr.readLineBlack(sensorValues);
    return position;
}


// ============================
// SETUP DE SENSORES
// ============================
void setupSensores(const uint8_t* sensorPins, uint8_t SensorCount) {
    qtr.setTypeAnalog();
    qtr.setSensorPins(sensorPins, SensorCount);
}