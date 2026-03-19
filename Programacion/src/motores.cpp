/**
 @file motores.cpp
 @brief Implementación de la capa de control de motores y manejo del driver DRV8833.
 @details Contiene la lógica para el control PWM de los motores, la gestión de dirección 
 (avance/reversa) y el cálculo de velocidades diferenciales basadas en la corrección PID.
 @author Legion de Ohm
 */

#include <Arduino.h>
#include "sensores.hpp"
#include "config.hpp"
#include "motores.hpp"
#include "pid.hpp"
#include "interrupciones.hpp"

/**
 @brief Constructor vacío de la clase Drv8833.
 */
Drv8833::Drv8833() {}

/**
 @brief Configura los pines de hardware y el periférico PWM del driver.
 @param pinIN1 Pin de dirección 1.
 @param pinIN2 Pin de dirección 2.
 @param pinSleep Pin de activación del driver.
 @param chPWM Canal ledc asignado.
 @param freqPWM Frecuencia del PWM.
 @param resPWM Resolución en bits del PWM.
 */
void Drv8833::setup(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinSleep, uint8_t chPWM, uint32_t freqPWM, uint8_t resPWM) {
    _pinIN1 = pinIN1;
    _pinIN2 = pinIN2;
    _pinSleep = pinSleep;
    _chPWM = chPWM;
    _freqPWM = freqPWM;
    _resPWM = resPWM;

    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);
    pinMode(_pinSleep, OUTPUT);

    digitalWrite(_pinSleep, HIGH); // Habilitar driver

    ledcSetup(_chPWM, _freqPWM, _resPWM);
}

/**
 @brief Mueve el motor hacia adelante.
 @details Desvincula el pin IN2 del PWM y lo pone en LOW, mientras vincula IN1 al canal PWM.
 @param pPWM Porcentaje de potencia [0-100].
 */
void Drv8833::forward(uint8_t pPWM) {
    ledcDetachPin(_pinIN2); 
    digitalWrite(_pinIN2, LOW);      

    ledcAttachPin(_pinIN1, _chPWM);

    uint16_t maxDuty = (1 << _resPWM) - 1;
    uint16_t pwmDutyCycle = (maxDuty * pPWM) / 100;

    ledcWrite(_chPWM, pwmDutyCycle);  
}

/**
 @brief Mueve el motor hacia atrás.
 @details Desvincula el pin IN1 del PWM y lo pone en LOW, mientras vincula IN2 al canal PWM.
 @param pPWM Porcentaje de potencia [0-100].
 */
void Drv8833::reverse(uint8_t pPWM) {
    ledcDetachPin(_pinIN1);
    digitalWrite(_pinIN1, LOW);

    ledcAttachPin(_pinIN2, _chPWM);

    uint16_t maxDuty = (1 << _resPWM) - 1;
    uint16_t pwmDutyCycle = (maxDuty * pPWM) / 100;

    ledcWrite(_chPWM, pwmDutyCycle);
}

/**
 @brief Detiene el motor desvinculando ambos pines del PWM y poniéndolos en LOW.
 */
void Drv8833::stop() {
    ledcDetachPin(_pinIN1);
    ledcDetachPin(_pinIN2);

    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, LOW);
}

// ============================
// CONFIGURACIÓN DE MOTORES
// ============================

/** @brief Instancia estática para el motor izquierdo. */
static Drv8833 motorIzq;
/** @brief Instancia estática para el motor derecho. */
static Drv8833 motorDer;

/** @brief Canal PWM para motor izquierdo. */
static const uint8_t motorPWM_Izq = 0;
/** @brief Canal PWM para motor derecho. */
static const uint8_t motorPWM_Der = 1;

/** @brief Frecuencia de trabajo del PWM (20 kHz). */
static const uint32_t freqPWM = 20000; 
/** @brief Resolución del PWM (8 bits). */
static const uint8_t  resPWM  = 8;     

/** @brief Almacena la velocidad calculada para el motor izquierdo. */
int32_t motorSpeedIzq = 0;
/** @brief Almacena la velocidad calculada para el motor derecho. */
int32_t motorSpeedDer = 0;

/**
 @brief Inicializa el hardware de ambos motores y los detiene inicialmente.
 */
void setupMotores() {
    motorDer.setup(motorPinIN1_Der, motorPinIN2_Der,
                   motorPinSleep_Der, motorPWM_Der,
                   freqPWM, resPWM);

    motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq,
                   motorPinSleep_Izq, motorPWM_Izq,
                   freqPWM, resPWM);

    detenerMotores();
}

/**
 @brief Aplica las velocidades a los motores traduciéndolas a comandos del driver.
 @details Evalúa el signo de la velocidad para decidir si aplicar forward, reverse o stop.
 @param motorSpeedIzq Velocidad para el motor izquierdo (positivo = avance).
 @param motorSpeedDer Velocidad para el motor derecho (positivo = avance).
 */
void moverMotores(int32_t motorSpeedIzq, int32_t motorSpeedDer) {
    deb(Serial.printf("MotorIzq=%d\n", motorSpeedIzq);)
    deb(Serial.printf("MotorDer=%d\n", motorSpeedDer);)

    if      (motorSpeedIzq > 0) {   motorIzq.forward(motorSpeedIzq);        }
    else if (motorSpeedIzq < 0) {   motorIzq.reverse(abs(motorSpeedIzq));   }
    else                        {   motorIzq.stop();    }

    if      (motorSpeedDer > 0) {   motorDer.forward(motorSpeedDer);        }
    else if (motorSpeedDer < 0) {   motorDer.reverse(abs(motorSpeedDer));   }
    else                        {   motorDer.stop();    }   
}

/**
 @brief Envía la señal de parada a ambos motores.
 */
void detenerMotores(){
    motorIzq.stop();
    motorDer.stop();
}

/**
 @brief Actualiza la bandera de Setpoint basada en la proximidad de la posición al centro.
 @param pos Posición actual detectada por los sensores.
 */
void actualizarSP(uint16_t pos) {
    SETPOINT = (abs(pos - setpoint) < zonaMuerta);
}

/**
 @brief Calcula las velocidades individuales aplicando la corrección diferencial.
 @details Si el robot está fuera de la zona muerta, ajusta las velocidades base 
 sumando o restando la corrección y limita los valores al rango @f$ \pm @f$maxSpeed.
 @param correcion Valor de corrección obtenido del PID.
 */
void controlMotores(float correcion) {
    if ( !SETPOINT ) {
        motorSpeedIzq = baseSpeed - correcion;
        motorSpeedDer = baseSpeed + correcion;

        motorSpeedIzq = constrain(motorSpeedIzq, -maxSpeed, maxSpeed);
        motorSpeedDer = constrain(motorSpeedDer, -maxSpeed, maxSpeed);
        return;
    }

    SETPOINT = true;
}
