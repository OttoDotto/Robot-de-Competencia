#include <Arduino.h>
#include "QTRSensors.h"
#include "drv8833.hpp"
#include "buzzer.hpp"
#include "interrupciones.hpp"
#include "config.hpp"
#include "pid.hpp"
#include "sensores.hpp"
#include "motores.hpp"


// VELOCIDADES  - PORCENTAJE DE PWM (0-100%)
const uint8_t maxSpeed  = 100;   // Límite de velocidad
int32_t motorSpeedIzq = baseSpeed;
int32_t motorSpeedDer = baseSpeed; 

// SETPOINT y ZONA MUERTA
uint16_t setpoint = 3500;
uint16_t zonaMuerta = 200;

float  lastError = 0;   // Error previo         -   control D
float  integral = 0;    // Acumulador           -   control I
uint32_t lastTime = 0;  // Millis previo        -   delta Tiempo

// =================================
// CONFIGURACIÓN DE BUZZER
// =================================
const uint8_t BuzzerPwm = 2; // Canal PWM 1
Buzzer buzzer(17, BuzzerPwm);

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
// CONFIGURACIÓN SENSORES QTR & LINEA
// ============================
const uint8_t S8 = 14;  const uint8_t S7 = 27;  const uint8_t S6 = 33;  const uint8_t S5 = 32;
const uint8_t S4 = 35;  const uint8_t S3 = 34;  const uint8_t S2 = 39;  const uint8_t S1 = 36;

QTRSensors qtr;         const uint8_t SensorCount = 8;
const uint8_t sensorPins[SensorCount] = {S8, S7, S6, S5, S4, S3, S2, S1};
uint16_t sensorValues[SensorCount];

uint16_t position;

// ============================
// LEDS Y BOTONES
// ============================
const uint8_t ledMotores = 13;          // LED para encender/apagar motores
const uint8_t ledCalibracion = 2;       // LED azul que indica calibración finalizada
const uint8_t BTN_RUN = 19;
const uint8_t BTN_STOP = 22;


// ============================
// SETUP
// ============================
void setup() {
    deb(Serial.begin(115200);)   
    mute(buzzer.begin();)         // 2 kHz y 8 bits

    // Configuración motores
    motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM_Der, freqPWM, resPWM);
    motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM_Izq, freqPWM, resPWM);

    // Configuración pines
    pinMode(ledMotores, OUTPUT);
    pinMode(ledCalibracion, OUTPUT);
    pinMode(BTN_RUN, INPUT);
    pinMode(BTN_STOP, INPUT);

    // Interrupciones de arranque y parada
    attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

    // Configuración sensores
    setupSensores(sensorPins, SensorCount);
    
    // Calibración inicial
    calibrarSensores();
}


// ============================
// LOOP
// ============================
void loop() {
    if (!RUN) {
        digitalWrite(ledMotores, LOW);
        motorIzq.stop();
        motorDer.stop();
        return;
    }

    // Enceder led modo corredor
    digitalWrite(ledMotores, HIGH);
    
    // Obtener el tiempo actual
    uint32_t now = tiempoActual();

    // Leer posición de línea (0 = extremo izquierda, 7000 = extremo derecha)    
    position = leerLinea();
    deb(Serial.printf("Posicion=%d\n", position);)  

    // calculo la correccion para los motores segun la posicion y el tiempo actual
    float correcion = calculo_pid(position, now);
 
    // Control de motores
    controlMotores(correcion);

    // Mover los motores (Avanza, retrocede o para)
    moverMotores(motorSpeedIzq, motorSpeedDer);

    //Guardar el tiempo al finalizar
    lastTime = now;
    deb(Serial.println("\n ---------------------- \n");)
}