#include <Arduino.h>

#include "QTRSensors.h"
#include "drv8833.hpp"
#include "buzzer.hpp"

#ifdef DEBUG
    // Hacemos lo que esta dentro
    #define deb(x) x
#else
    // No hacemos lo que esta dentro
    #define deb(x)
#endif


// ===================================
// SELECCION DE CORREDOR - CAMBIAR EN PLATFORMIO.INI
// ===================================
#define NIGHTFALL 1
#define ARGENTUM  2
#define DIEGO     3

#ifndef CORREDOR
  #error "Debe definir CORREDOR en platformio.ini (ej: -D CORREDOR=NIGHTFALL)"
#endif

// Selección de parametros segun corredor
#if (CORREDOR == NIGHTFALL)
    #define USAR_MICROS   1         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 95;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.1;
    const float Tu = 0.9;
#elif (CORREDOR == ARGENTUM)
    #define USAR_MICROS   0         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 85;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.05;
    const float Tu = 1.100;
#elif (CORREDOR == DIEGO)
    #define USAR_MICROS   0         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 90;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.049;
    const float Tu = 0.72;
#else
  #error "Valor de CORREDOR inválido. Use NIGHTFALL, ARGENTUM o DIEGO."
#endif


// =================================
// CONFIGURACIÓN DE TIEMPOS
// =================================
#if USAR_MICROS
  #define tiempoActual() micros()
  const float TIME_DIVISOR = 1000000.0; // microsegundos → segundos
#else
  #define tiempoActual() millis()
  const float TIME_DIVISOR = 1000.0;    // milisegundos → segundos
#endif


// ============================
// VELOCIDADES  - PORCENTAJE DE PWM (0-100%)
// ============================
const uint8_t maxSpeed  = 100;   // Límite de velocidad
int32_t motorSpeedIzq = baseSpeed;
int32_t motorSpeedDer = baseSpeed; 


// ============================
// CONTROL PID - METODO Ziegler-Nichols
// ============================
#ifdef TEST_PID
    const float Kp = Ku;
    const float Ki = 0;
    const float Kd = 0;
    #define debTestPid(x)       // Desactivamos la bocina
#else
    const float Kp = 0.6 * Ku;
    const float Ki = 2 * Kp / Tu;
    const float Kd = Kp * Tu / 8;
    #define debTestPid(x) x         // Activamos la bocina
#endif

uint16_t setpoint = 3500;
uint16_t zonaMuerta = 200; // 3500 +- 200

float  lastError = 0;   // Error previo         -   control D
float  integral = 0;    // Acumulador           -   control I
uint32_t lastTime = 0;  // Millis previo        -   delta Tiempo


// =================================
// CONFIGURACIÓN DE BUZZER
// =================================
debTestPid(
    const uint8_t BuzzerPwm = 2; // Canal PWM 1
    Buzzer buzzer(17, BuzzerPwm);
)

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

QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

uint16_t position;

enum Linea { BLANCA, NEGRA };
Linea linea_competencia = BLANCA;   // CAMBIAR BLANCA o NEGRA de acorde a la linea



// ============================
// LEDS Y BOTONES
// ============================
const uint8_t ledMotores = 13;          // LED para encender/apagar motores
const uint8_t ledCalibracion = 2;       // LED azul que indica calibración finalizada
const uint8_t BTN_RUN = 19;
const uint8_t BTN_STOP = 22;


// ============================
// INTERRUPCIONES
// ============================
volatile bool RUN = false;
void IRAM_ATTR handleRun()  {    RUN = true;    }
void IRAM_ATTR handleStop() {    RUN = false;   }


// ============================
// FUNCION CALIBRAR
// ============================
void calibrarSensores() {
    debTestPid(
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

    debTestPid(
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
// FUNCION MOVER MOTORES
// ============================
void moverMotores(int16_t motorSpeedIzq, int16_t motorSpeedDer) {
    deb(Serial.printf("MotorIzq=%d\n", motorSpeedIzq);)
    deb(Serial.printf("MotorDer=%d\n", motorSpeedDer);)

    if      (motorSpeedIzq > 0) {   motorIzq.forward(motorSpeedIzq);        }
    else if (motorSpeedIzq < 0) {   motorIzq.reverse(abs(motorSpeedIzq));   }
    else                        {   motorIzq.stop();    }

    if      (motorSpeedDer > 0) {   motorDer.forward(motorSpeedDer);        }
    else if (motorSpeedDer < 0) {   motorDer.reverse(abs(motorSpeedDer));   }
    else                        {   motorDer.stop();    }   
}


// ============================
// FUNCION CALCULO DE PID
// ============================
float calculo_pid(uint16_t pos, uint32_t now) {
    // Calcular deltaTime
    float  deltaTime = (now - lastTime) / TIME_DIVISOR; // Convertir a segundos
    deb(Serial.printf("deltaTime=%.6f\n", deltaTime);)

    // Calcular el error
    float  error = pos - setpoint;               // valores entre -3500 y 3500
    deb(Serial.printf("error=%.6f\n", error);)

    // Calcular derivativo (tasa de cambio del error)
    float  derivativo = (error - lastError) / deltaTime;

    // Actualizar el último error proporcional
    lastError = error;

    // Calcular integral (acumulación del error)
    integral += error * deltaTime;

    // Calcular la salida del PID
    float  output = (error * Kp) + (derivativo * Kd) + (integral * Ki);

    deb(Serial.printf("PID=%.6f\n", output);)
    return output;
}


// ============================
// FUNCION CONTROL MOTORES - Zona muerta o Pid
// ============================
void controlMotores(float correcion) {
    // En Zona muerta dejo de acumular y solo acelero
    if (abs(position - setpoint) < zonaMuerta) {
        motorSpeedIzq++;    // aumento las velocidades lentamente
        motorSpeedDer++;
        
        correcion = 0;      // no corregir
        integral = 0;       // dejar de acumular I
    }
    else{
        // Calcular velocidad motores
        motorSpeedIzq = baseSpeed - correcion;
        motorSpeedDer = baseSpeed + correcion;
    }
    // Limitar a rango válido incluyendo negativos
    motorSpeedIzq = constrain(motorSpeedIzq, -maxSpeed, maxSpeed);
    motorSpeedDer = constrain(motorSpeedDer, -maxSpeed, maxSpeed);
}


// ============================
// SETUP
// ============================
void setup() {
    deb(Serial.begin(115200);)   
    debTestPid(buzzer.begin();)         // 2 kHz y 8 bits

    // Configuración motores
    motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM_Der, freqPWM, resPWM);
    motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM_Izq, freqPWM, resPWM);

    // Configuración sensores
    qtr.setTypeAnalog();
    const uint8_t sensorPins[SensorCount] = {S8, S7, S6, S5, S4, S3, S2, S1};
    qtr.setSensorPins(sensorPins, SensorCount);

    // Configuración pines
    pinMode(ledMotores, OUTPUT);
    pinMode(ledCalibracion, OUTPUT);
    pinMode(BTN_RUN, INPUT);
    pinMode(BTN_STOP, INPUT);

    // Interrupciones de arranque y parada
    attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

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