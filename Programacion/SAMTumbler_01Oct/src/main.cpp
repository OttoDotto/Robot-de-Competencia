#include <Arduino.h>
#include "drv8833.hpp"
#include "QTRSensors.h"
#include "InterrupcionesRobot.hpp"
#include "buzzer.hpp"
#include "Pitches.hpp"
#include "PIDController.hpp"

// MOTOR IZQUIERDO
// Pines
const uint8_t motorPinIN1_Izq = 18;
const uint8_t motorPinIN2_Izq = 21;
const uint8_t motorPinSleep_Izq = 23;

// MOTOR DERECHO
// Pines
const uint8_t motorPinIN1_Der = 26;
const uint8_t motorPinIN2_Der = 25;
const uint8_t motorPinSleep_Der = 16;

// Canales PWM
const uint8_t motorPWM = 0; // Canal PWM 0
// Ajustes PWM
const uint32_t freqPWM = 10000; // Frecuencia del PWM = 10KHz
const uint8_t resPWM = 8;       // Resolución de 8 bits = 256 valores posibles [0, 255]

// Creación de dos objetos de la clase Motor
Drv8833 motorDer;
Drv8833 motorIzq;

const uint8_t BuzzerPwm = 1; // Canal PWM 1
Buzzer buzzer(17, BuzzerPwm);

const int LED_INTERNAL = 2;
const int LED_EXTERNAL = 13;
const int BTN_RUN = 22;
const int BTN_STOP = 19;

InterrupcionesRobot robot(LED_INTERNAL, LED_EXTERNAL, BTN_RUN, BTN_STOP);

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
  
  for (uint16_t i = 0; i < 400; i++) {  qtr.calibrate(); }
  
  Serial.println("Calibracion lista!");

  buzzer.play(NOTE_A4);
  delay(200);
  buzzer.stop();
}

void correr() {
  Serial.println("[Corredor] motores corriendo...");
  // Mover los motores hacia adelante al 50% del PWM
  motorIzq.forward(50);
  motorDer.forward(50);

  /*
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
  */
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
