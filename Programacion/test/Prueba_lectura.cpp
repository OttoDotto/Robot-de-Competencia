#include <Arduino.h>
#include "QTRSensors.h"

// ----------------------------
// CONFIGURACIÓN
// ----------------------------
#define USE_RC  // Comentar para usar ADC
#define LED_BUILTIN 2

const uint8_t SensorCount = 8;
const uint8_t sensorPins[SensorCount] = {14, 27, 33, 32, 35, 34, 39, 36};
uint16_t sensorValues[SensorCount];
uint16_t position;

QTRSensors qtr;

unsigned long previousMillis = 0;
const long interval = 10; // Leer cada 10 ms

// ----------------------------
// SETUP
// ----------------------------
void setup() {
    Serial.begin(115200);

    // Configurar tipo de sensor
    #ifdef USE_RC
        qtr.setTypeRC();
    #else
        qtr.setTypeAnalog();
        qtr.setEmitterPin(2); // Opcional solo para ADC
    #endif

    // Configurar pines
    qtr.setSensorPins(sensorPins, SensorCount);

    // Calibración inicial
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Calibrando sensores... mueve el sensor sobre la linea negra y fondo");

    for (uint16_t i = 0; i < 400; i++) {
        qtr.calibrate();
        delay(25); // opcional para no saturar el ADC/RC
    }

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Calibración finalizada.");
}

// ----------------------------
// LOOP
// ----------------------------
void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Lectura de sensores
        #ifdef USE_RC
            qtr.read(sensorValues);                     // Leer valores RC
            position = qtr.readLineBlack(sensorValues);
        #else
            position = qtr.readLineWhite(sensorValues);
        #endif

        // Mostrar valores y posición
        for (uint8_t i = 0; i < SensorCount; i++) {
            Serial.print(sensorValues[i]);
            Serial.print('\t');
        }
        Serial.print(" | Posicion: ");
        Serial.println(position);
    }
}
