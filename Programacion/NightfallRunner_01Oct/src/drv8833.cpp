/**
 * \file drv8833.cpp
 * \brief Implementación para el driver de motores DRV8833
 * \author Legion de Ohm
 */

#include <Arduino.h>
#include "drv8833.hpp"

Drv8833::Drv8833() 
{
    pinMode(_pinSleep, OUTPUT);     // Configuración de pinSleep como salida.
    digitalWrite(_pinSleep, HIGH);  // LOW lo desactiva. HIGH lo activa
}

void Drv8833::setup(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinSleep, uint8_t chPWM, uint32_t freqPWM, uint8_t resPWM)
{
    // Ajuste de variables
    _pinIN1 = pinIN1;
    _pinIN2 = pinIN2;
    _pinSleep = pinSleep;
    _chPWM = chPWM;
    _freqPWM = freqPWM;
    _resPWM = resPWM;

    // Configuración de pines.
    pinMode(_pinIN1, OUTPUT); // Configuración de pinIN1 como salida.
    pinMode(_pinIN2, OUTPUT); // Configuración de pinIN2 como salida.

    // Ajuste de PWM.
    ledcSetup(_chPWM, _freqPWM, _resPWM); // Se configura la frecuencia y la resolución del canal PWM.
}

void Drv8833::forward(uint8_t pPWM)
{
    ledcDetachPin(_pinIN2);             // Separo el pin IN2 del PWM a puramente DIGITAL.
    digitalWrite(_pinIN2, 0);           // Aplico un bajo en el pin IN2.

    ledcAttachPin(_pinIN1, _chPWM);     // Se conecta el pin IN1 al canal de PWM.

    // Calculo del ciclo de trabajo del PWM.
    // * pwmDutyCycle depende de la resolucion y %PWM
    // ~ pow(2, _resPWM) = 2^resolucion 
    // ~ %PWM / 100.0 = nro entre 0 y 1
    // ~ Round(...) = redondeo
    uint16_t pwmDutyCycle;
    pwmDutyCycle = round(pow(2, _resPWM) * (pPWM / 100.0));
    
    ledcWrite(_chPWM, pwmDutyCycle);    // Aplico el ciclo de trabajo al PWM del canal.
}

void Drv8833::reverse(uint8_t pPWM)
{
    ledcDetachPin(_pinIN1);             // Separo el pin IN1 del PWM a puramente DIGITAL.
    digitalWrite(_pinIN1, 0);           // Aplico un bajo en el pin IN1.

    ledcAttachPin(_pinIN2, _chPWM);     // Se conecta el pin IN2 al canal de PWM.

    // Calculo del ciclo de trabajo del PWM
    // * pwmDutyCycle depende de la resolucion y %PWM
    // ~ pow(2, _resPWM) = 2^resolucion 
    // ~ %PWM / 100.0 = nro entre 0 y 1
    // ~ Round(...) = redondeo
    uint16_t pwmDutyCycle;
    pwmDutyCycle = round(pow(2, _resPWM) * (pPWM / 100.0));

    ledcWrite(_chPWM, pwmDutyCycle);    // Aplico el ciclo de trabajo al PWM del canal.
}

void Drv8833::stop()
{
    // Paso los pines IN1 y IN2 a puramente digitales
    ledcDetachPin(_pinIN1);
    ledcDetachPin(_pinIN2);

    // Asigno un bajo a los pines, deteniendo ambos motores.
    digitalWrite(_pinIN1, 0);
    digitalWrite(_pinIN2, 0);
}
