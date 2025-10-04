#include <Arduino.h>
#include "drv8833.hpp"
#include "QTRSensors.h"
#include "buzzer.hpp"
#include "Pitches.hpp"

// ============================
// CONFIGURACIÓN DE MOTORES
// ============================ 
const uint8_t motorPinIN1_Izq = 21;   const uint8_t motorPinIN2_Izq = 18;
const uint8_t motorPinSleep_Izq = 23;

// MOTOR DERECHO
const uint8_t motorPinIN1_Der = 26;   const uint8_t motorPinIN2_Der = 25;
const uint8_t motorPinSleep_Der = 16;

// Ajustes PWM MOTORES
const uint8_t motorPWM_Izq = 0; const uint8_t motorPWM_Der = 1;
const uint32_t freqPWM = 20000; // Frecuencia del PWM = 20KHz
const uint8_t resPWM = 8;       // Resolución de 8 bits = 256 valores posibles [0, 255]

Drv8833 motorDer;   Drv8833 motorIzq;

// ============================
// CONFIGURACIÓN SENSORES QTR
// ============================
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// ============================
// CONTROL PID
// ============================
float Kp = 0.0;
float Ki = 0.000012;
float Kd = 0.0;
const float KpRecta = 0.0019;
const float KpCurva = 0.019;
const float KpCurvaCerrada = 0.024;

// ============================
// VELOCIDADES
// ============================
int16_t baseSpeed = 75; 
int16_t baseSpeedRecta = 85; //90;
int16_t baseSpeedCurva = 75; //75;
int16_t baseSpeedCurvaCerrada = 65; //60; 
int16_t maxSpeed  = 95;  // Límite de PWM
long lastError = 0; 
long integral = 0;

// ====================================
// INTERVALO MILLIS 
// ======================================= 
unsigned long previousMillis = 0; 
const long interval = 10; // Loop cada 10 ms

// =================================
// CONFIGURACIÓN DE BUZZER
// =================================
const uint8_t BuzzerPwm = 2; // Canal PWM 1
Buzzer buzzer(17, BuzzerPwm);

// ============================
// LEDS Y BOTONES - INTERRUPCIONES
// ============================
const int ledMotores = 13;          // LED para encender/apagar motores
const int ledCalibracion = 2;       // LED azul que indica calibración finalizada
const int BTN_RUN = 19;
const int BTN_STOP = 22;

// INTERRUPCIONES
bool RUN = false;
void IRAM_ATTR handleRun()  {RUN = true;}
void IRAM_ATTR handleStop() {RUN = false;}


// ============================
// FUNCION CALIBRAR
// ============================
void calibrarSensores()
{
    buzzer.play(NOTE_A4);
    delay(150);
    buzzer.stop();

    motorIzq.stop();
    motorDer.stop();

    // Serial.println("Calibrando sensores..."); 
    digitalWrite(ledCalibracion, HIGH);

    for (uint16_t i = 0; i < 350; i++) { qtr.calibrate(); } 

    // Serial.println("Calibracion lista!");
    digitalWrite(ledCalibracion, LOW);

    buzzer.play(NOTE_C5);
    delay(200);
    buzzer.stop();
}


// ============================
// SETUP
// ============================
void setup() {
    //Serial.begin(115200);
    buzzer.begin();         // 2 kHz y 8 bits

    // Configuración motores
    motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM_Der, freqPWM, resPWM);
    motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM_Izq, freqPWM, resPWM);

    // Configuración sensores
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){14, 27, 33, 32, 35, 34, 39, 36}, SensorCount);

    // Configuración pines
    pinMode(ledMotores, OUTPUT);
    pinMode(ledCalibracion, OUTPUT);
    pinMode(BTN_RUN, INPUT);            // botones pulldown
    pinMode(BTN_STOP, INPUT);

    // Interrupciones de arranque y parada
    attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

    // Calibración
    calibrarSensores();
}


// ============================
// LOOP
// ============================
void loop() {
    if (!RUN)
    {
        digitalWrite(ledMotores, LOW);
        motorIzq.stop();
        motorDer.stop();
        return;
    }

    digitalWrite(ledMotores, HIGH);
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {   // Leer sensores cada 'interval' ms 
        previousMillis = currentMillis;  // Leer posición de línea (0 = extremo izquierda, 7000 = extremo derecha)
        
        uint16_t position = qtr.readLineBlack(sensorValues);
        // Serial.println(position);
        // usar para linea blanca:  qtr.readLineWhite(sensorValues);
        // usar para linea blanca: qtr.readLineBlack(sensorValues); 

        float error = position - 3500; // Centro de 8 sensores = 3500

    if (abs(error) < 500) { // Recta 
        baseSpeed = baseSpeedRecta; 
        Kp = KpRecta;
        }
    else if (abs(error) < 1500) { // Curva media
        baseSpeed = baseSpeedCurva; 
        Kp = KpCurva;
    }
    else { // Curva cerrada
        baseSpeed = baseSpeedCurvaCerrada; 
        Kp = KpCurvaCerrada;
    }

    float P = Kp * error; // Proporcional
    integral += error;
    integral = constrain(integral, -10000, 10000); //Limita acumulación
    float I = Ki * integral;
    //float D = Kd * (error - lastError);

    float PIDvalue = P + I; //+ D;

    lastError = error;

    // Calcular velocidad motores
    int16_t motorSpeedIzq = baseSpeed - PIDvalue;
    int16_t motorSpeedDer = baseSpeed + PIDvalue;

    // Limitar a rango válido incluyendo negativos
    motorSpeedIzq = constrain(motorSpeedIzq, -maxSpeed, maxSpeed);
    motorSpeedDer = constrain(motorSpeedDer, -maxSpeed, maxSpeed);

    // --- Control de motores ---
    if (motorSpeedIzq > 0) {    motorIzq.forward(motorSpeedIzq);    }
    else if (motorSpeedIzq < 0) {
        motorSpeedIzq = motorSpeedIzq - 28;
        motorIzq.reverse(abs(motorSpeedIzq));
    }
    else {  motorIzq.stop(); }

    if (motorSpeedDer > 0)  {    motorDer.forward(motorSpeedDer);    }
    else if (motorSpeedDer < 0) {
        motorSpeedDer = motorSpeedDer - 28;
        motorDer.reverse(abs(motorSpeedDer));
    }
    else {  motorDer.stop();    }
    }
}