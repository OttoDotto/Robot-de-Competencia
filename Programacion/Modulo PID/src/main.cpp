#include <Arduino.h>
#include "drv8833.hpp"
#include "QTRSensors.h"
#include "PIDController.hpp"

// ================================= // LEDS // =================================
const int ledMotores = 13;
const int ledCalibracion = 2;

// ================================= // CONFIGURACIÓN DE MOTORES // =================================
const uint8_t motorPinIN1_Izq = 18; 
const uint8_t motorPinIN2_Izq = 21; 
const uint8_t motorPinSleep_Izq = 23;

const uint8_t motorPinIN1_Der = 26; 
const uint8_t motorPinIN2_Der = 25; 
const uint8_t motorPinSleep_Der = 16;

const uint8_t motorPWM_Izq = 0; 
const uint8_t motorPWM_Der = 1;

const uint32_t freqPWM = 20000; 
const uint8_t resPWM = 8;

Drv8833 motorDer; 
Drv8833 motorIzq;

// ================================= // CONFIGURACIÓN DE SENSORES QTR // =================================
QTRSensors qtr;
const uint8_t SensorCount = 8; 
uint16_t sensorValues[SensorCount];

// ================================= // CONTROL PID // =================================
const float KpRecta = 0.005;
const float KpCurva = 0.015;
const float KpCurvaCerrada = 0.03;

PIDController pid(KpRecta, 0.0, 0.0);

int16_t baseSpeed = 50; 
int16_t baseSpeedRecta = 82;
int16_t baseSpeedCurva = 65;
int16_t baseSpeedCurvaCerrada = 50; 
int16_t maxSpeed  = 80;

// ================================= // INTERVALO LOOP // =================================
unsigned long previousMillis = 0; 
const long interval = 10;

// ================================= // SETUP // =================================
void setup() {
  Serial.begin(115200);

  // Motores
  motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM_Der, freqPWM, resPWM); 
  motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM_Izq, freqPWM, resPWM);

  // Sensores
  qtr.setTypeAnalog(); 
  qtr.setSensorPins((const uint8_t[]){14, 27, 33, 32, 35, 34, 39, 36}, SensorCount);

  // Calibración
  Serial.println("Calibrando sensores...");
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  Serial.println("Calibracion lista!");

  delay(500);
}

// ================================= // LOOP PRINCIPAL // =================================
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Leer posición de línea
    uint16_t position = qtr.readLineWhite(sensorValues);
    float error = position - 3500;  // centro de 8 sensores = 3500

    // Ajuste de velocidad y Kp dinámico
    if (abs(error) < 500) {
      baseSpeed = baseSpeedRecta; 
      pid.setTunings(KpRecta, 0, 0);
    } else if (abs(error) < 1500) {
      baseSpeed = baseSpeedCurva; 
      pid.setTunings(KpCurva, 0, 0);
    } else {
      baseSpeed = baseSpeedCurvaCerrada; 
      pid.setTunings(KpCurvaCerrada, 0, 0);
    }

    // Calcular PID
    float PIDvalue = pid.compute(error);

    // Velocidades motores
    int16_t motorSpeedIzq = constrain(baseSpeed - PIDvalue, 0, maxSpeed);
    int16_t motorSpeedDer = constrain(baseSpeed + PIDvalue, 0, maxSpeed);

    if (motorSpeedIzq < 50 && motorSpeedIzq > 0) motorSpeedIzq = 0;
    if (motorSpeedDer < 50 && motorSpeedDer > 0) motorSpeedDer = 0;

    // Control motores
    if (motorSpeedIzq > 0) motorIzq.forward(motorSpeedIzq);
    else motorIzq.stop();

    if (motorSpeedDer > 0) motorDer.forward(motorSpeedDer);
    else motorDer.stop();

    // Debug
    Serial.print(">error:"); Serial.print(error);
    Serial.print(" PWM_Izq:"); Serial.print(motorSpeedIzq);
    Serial.print(" PWM_Der:"); Serial.println(motorSpeedDer);
  }
}
