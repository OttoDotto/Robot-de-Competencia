#include <Arduino.h>
#include "drv8833.hpp"
#include "QTRSensors.h"
#include "InterrupcionesRobot.hpp"
#include "buzzer.hpp"
#include "Pitches.hpp"
#include "PIDController.hpp"

// ================================= // CONFIGURACIÓN DE MOTORES // =================================
// MOTOR IZQUIERDO
const uint8_t motorPinIN1_Izq = 21;   const uint8_t motorPinIN2_Izq = 18;
const uint8_t motorPinSleep_Izq = 23;

// MOTOR DERECHO
const uint8_t motorPinIN1_Der = 26;   const uint8_t motorPinIN2_Der = 25;
const uint8_t motorPinSleep_Der = 16;

// Ajustes PWM MOTORES
const uint8_t motorPWM = 0; // Canal PWM 0
const uint32_t freqPWM = 10000; // Frecuencia del PWM = 10KHz
const uint8_t resPWM = 8;       // Resolución de 8 bits = 256 valores posibles [0, 255]

Drv8833 motorDer;
Drv8833 motorIzq;

// ================================= // CONFIGURACIÓN DE BUZZER // =================================
const uint8_t BuzzerPwm = 1; // Canal PWM 1
Buzzer buzzer(17, BuzzerPwm);

// ================================= // CONFIGURACIÓN DE INTERRUPCIONES  // =================================
const int LED_INTERNAL = 2;
const int LED_EXTERNAL = 13;
const int BTN_RUN = 22;
const int BTN_STOP = 19;

InterrupcionesRobot robot(LED_INTERNAL, LED_EXTERNAL, BTN_RUN, BTN_STOP);

// ================================= // CONFIGURACIÓN DE SENSORES QTR // =================================
QTRSensors qtr;
const uint8_t SensorCount = 8; 
uint16_t sensorValues[SensorCount];

// ================================= // CONTROL PID ADAPTATIVO // =================================
const float KpRecta = 0.2;          // rectas
const float KpCurva = 0.1;          // curvas abiertas
const float KpCurvaCerrada = 0.1;   // curvas cerradas

PIDController pid(KpRecta, 0.0, 0.0);

int16_t baseSpeed = 90;
int16_t baseSpeedRecta = 95;        // % PWM
int16_t baseSpeedCurva = 85;
int16_t baseSpeedCurvaCerrada = 70;
int16_t maxSpeed  = 100;

const long interval = 5;            // cada 5 ms
unsigned long previousMillis = 0;


void correr() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //Calcula un promedio ponderado de las lecturas de todos los sensores. (0 = extremo izquierda, 7000 = extremo derecha)
    uint16_t position = qtr.readLineWhite(sensorValues);    // Leer posición de ínea negra
    
    // uint16_t position = qtr.readLineBlack(sensorValues); // Leer posición de ínea negra

    float error = position - 3500;

    // Selección de velocidad y Kp
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

    // PID solo proporcional
    int PIDvalue = int(pid.compute(error));

    // Velocidades motores (%)
    int16_t motorSpeedIzq = constrain(baseSpeed - PIDvalue, 0, maxSpeed);
    int16_t motorSpeedDer = constrain(baseSpeed + PIDvalue, 0, maxSpeed);

    // Control motores
    if (motorSpeedIzq > 0) motorIzq.forward(motorSpeedIzq);
    else motorIzq.stop();

    if (motorSpeedDer > 0) motorDer.forward(motorSpeedDer);
    else motorDer.stop();

    // Debug
    Serial.print(">error:"); Serial.print(error);
    Serial.print(" PID:"); Serial.print(PIDvalue);
    Serial.print(" PWM_Izq:"); Serial.print(motorSpeedIzq);
    Serial.print(" PWM_Der:"); Serial.println(motorSpeedDer);
  }
}


void parar() {
  Serial.println("[Parado] motores apagados...");
    
  // Detener motores
  motorIzq.stop();
  motorDer.stop();
  delay(20);
}


void calibracion() {
  parar();

  buzzer.play(NOTE_C4);
  delay(200);
  buzzer.stop();

  Serial.println("[Calibración] sensores calibrando...");
  
  for (uint16_t i = 0; i < 200; i++) {  qtr.calibrate(); }
  
  Serial.println("Calibracion lista!");

  buzzer.play(NOTE_A4);
  delay(200);
  buzzer.stop();
}


void setup()
{
  Serial.begin(115200);

  // Configuración motores IZQ y DER
  motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM, freqPWM, resPWM);
  motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM, freqPWM, resPWM);

  // Configuración sensores 
  qtr.setTypeAnalog(); 
  qtr.setSensorPins((const uint8_t[]){14, 27, 33, 32, 35, 34, 39, 36}, SensorCount); 

  // Configuración interrupciones
  robot.begin();
  robot.onCalibration(calibracion);
  robot.onRunning(correr);
  robot.onStopped(parar);

  // Configuración buzzer
  buzzer.begin();
}

void loop()
{
  // El programa se maneja mediante las interrupciones (botones)
  // Boton derecho (SW-PAR) - CALIBRACION
  // Boton izquierdo (SW-IMPAR) - CORRER 
  robot.update();
}
