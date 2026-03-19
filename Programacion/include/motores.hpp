/**
 @file motores.hpp
 @brief Cabecera de la capa de control de motores de alto nivel. Contiene las declaraciones de las variables de velocidad globales, las instancias de los drivers DRV8833, y las funciones que aplican la corrección PID para mover el robot.
 @author Legion de Ohm
 */

#pragma once

#ifndef DRV8833_H
#define DRV8833_H

/**
 @class Drv8833
 @brief Clase para drivers de motores DRV8833. Incluye funciones que permiten configurar sus pines, ajustar el PWM y el movimiento de los motores.
 */
class Drv8833 {
public:
    /**
     @brief Constructor de la clase Drv8833. Inicializa el objeto y prepara la habilitación de los motores.
     */
    Drv8833();

    /**
     @brief Configura los pines de control del motor y los parámetros del PWM.
     @param pinIN1 GPIO conectado al pin IN1 del driver.
     @param pinIN2 GPIO conectado al pin IN2 del driver.
     @param pinSleep GPIO conectado al pin Sleep del driver (control de modo de bajo consumo).
     @param chPWM Canal PWM de hardware a utilizar (ESP32).
     @param freqPWM Frecuencia de la señal PWM en Hz.
     @param resPWM Resolución de la señal PWM en bits.
     @return void
     */
    void setup(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinSleep,
                uint8_t chPWM, uint32_t freqPWM, uint8_t resPWM);

    /**
     @brief Mueve el motor hacia adelante con una potencia específica.
     @param pPWM Porcentaje de potencia a aplicar [0 a 100] %.
     @return void
     */
    void forward(uint8_t pPWM);

    /**
     @brief Mueve el motor hacia atrás con una potencia específica.
     @param pPWM Porcentaje de potencia a aplicar [0 a 100] %.
     @return void
     */
    void reverse(uint8_t pPWM);

    /**
     @brief Detiene el motor por completo.
     @return void
     */
    void stop();

private:
    uint8_t _pinIN1;   ///< GPIO del pin IN1.
    uint8_t _pinIN2;   ///< GPIO del pin IN2.
    uint8_t _pinSleep; ///< GPIO del pin Sleep (Habilitación).
    uint8_t _chPWM;    ///< Canal PWM configurado.
    uint32_t _freqPWM; ///< Frecuencia PWM en Hz.
    uint8_t _resPWM;   ///< Resolución PWM en bits.
};
#endif

/**
 @var motorSpeedIzq
 @brief Velocidad final del motor izquierdo calculada tras el PID. Positivo = avance, Negativo = reversa.
 */
extern int32_t motorSpeedIzq;

/**
 @var motorSpeedDer
 @brief Velocidad final del motor derecho calculada tras el PID. Positivo = avance, Negativo = reversa.
 */
extern int32_t motorSpeedDer;

/**
 @var maxSpeed
 @brief Límite máximo de PWM (Saturación) para proteger los motores y la fuente de poder.
 */
extern const int32_t maxSpeed;

/**
 @brief Inicializa los objetos de los motores y configura sus periféricos.
 @return void
 */
void setupMotores();

/**
 @brief Mueve los motores aplicando directamente los valores de velocidad. Esta función es la interfaz de bajo nivel que llama a las funciones `forward` / `reverse`.
 @param motorSpeedIzq Velocidad (PWM) final para el motor izquierdo.
 @param motorSpeedDer Velocidad (PWM) final para el motor derecho.
 @return void
 */
void moverMotores(int32_t motorSpeedIzq, int32_t motorSpeedDer);

/**
 @brief Detiene ambos motores de forma inmediata enviando una señal de parada a los drivers.
 @return void
 */
void detenerMotores();

/**
 @brief Función principal de control que calcula la velocidad final de cada motor basándose en la corrección PID.
 @param correcion Valor de salida del algoritmo PID (error corregido).
 @return void
 */
void controlMotores(float correcion);

/**
 @brief Actualiza el SetPoint (punto de referencia) del sistema de control según la posición actual del robot.
 @param pos Posición actual leída por la barra de sensores.
 @return void
 */
void actualizarSP(uint16_t pos);
