#include <Arduino.h>
#include "QTRSensors.h"

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

unsigned long previousMillis = 0;
const long interval = 10; // Leer cada 10 ms

void setup() {
  Serial.begin(115200);

  // Configurar sensores analógicos en pines ESP32
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){14, 27, 33, 32, 35, 34, 39, 36}, SensorCount);
  qtr.setEmitterPin(2); // Pin opcional para controlar LEDs del sensor

  delay(500);

  // Calibración inicial
  Serial.println("Calibrando sensores... mueve el sensor sobre linea negra y fondo");
  for (uint16_t i = 0; i < 400; i++) { // ~10 segundos
    qtr.calibrate();
  }
  Serial.println("Calibracion finalizada.");
}

void loop() {
  unsigned long currentMillis = millis();

  // Leer sensores cada 'interval' ms
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Leer línea negra calibrada
    //uint16_t position = qtr.readLineBlack(sensorValues);
    uint16_t position = qtr.readLineWhite(sensorValues); // usar readLineBlack si línea negra
        

    // Mostrar valores en monitor serial
    for (uint8_t i = 0; i < SensorCount; i++) {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    Serial.println(position);
  }
}
