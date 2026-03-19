/**
 @file pid.hpp
 @brief Cabecera para las variables y funciones del controlador Proporcional-Integral-Derivativo (PID). Contiene las declaraciones de las variables de estado (error, integral, tiempo) y la función principal para calcular la corrección necesaria para seguir la línea.
 @author Legion de Ohm
 */

#pragma once
#include <Arduino.h>

/**
 @var setpoint
 @brief Valor de referencia (punto de consigna) que representa la posición central sobre la línea.
 */
extern uint16_t setpoint;

/**
 @var zonaMuerta
 @brief Rango de error dentro del cual el controlador no realiza correcciones para evitar oscilaciones innecesarias.
 */
extern uint16_t zonaMuerta;

/**
 @var baseSpeed
 @brief Velocidad crucero nominal de los motores cuando el error es mayor a la zona muerta.
 */
extern uint8_t baseSpeed;

// ============================
// CONTROL PID - METODO Ziegler-Nichols
// ============================

/**
 @name Parámetros de Sintonización
 @brief Constantes calculadas mediante el método de Ziegler-Nichols para estabilizar el sistema.
 @{
 */
extern const float Ku; ///< Ganancia última (Ultimate Gain).
extern const float Tu; ///< Periodo de oscilación última (Ultimate Period).

extern const float Kp; ///< Constante Proporcional.
extern const float Ki; ///< Constante Integral.
extern const float Kd; ///< Constante Derivativa.
///@}

/**
 @brief Calcula la corrección necesaria para el sistema usando la fórmula PID. 
 @details La salida es la corrección que se suma o resta de la velocidad base de los motores. 
 La fórmula general aplicada es: 
 @f[ \text{Salida} = K_p \cdot \text{Error} + K_i \cdot \int \text{Error} \, dt + K_d \cdot \frac{d\text{Error}}{dt} @f]
 @param pos Posición actual leída por los sensores.
 @param deltaTime Tiempo transcurrido (@f$ \Delta T @f$) desde la última ejecución en segundos.
 @return float Valor de corrección PID a aplicar a las velocidades de los motores.
 */
float calculo_pid(uint16_t pos, float deltaTime);

/**
 @brief Resetea las variables internas del controlador (error acumulado y error anterior).
 @details Es fundamental llamar a esta función al reiniciar la marcha o tras una parada para evitar el "Windup" de la parte integral.
 @return void
 */
void reiniciar_pid();