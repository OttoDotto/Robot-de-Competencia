/**
 @file config.cpp
 @brief Implementación y asignación física de los pines del microcontrolador.
 @details Define la conexión de periféricos de interfaz, motores y el array de sensores.
 @author Legion de Ohm
 */

#include "config.hpp"


// ============================
// PINES - LEDS, BOTONES, BOCINA y SENSOR IR
// ============================
/** @brief Pin GPIO asignado al zumbador. */
const uint8_t pinBuzzer = 17;

/** @brief LED indicador para el estado de los motores. */
const uint8_t ledMotores = 13;

/** @brief LED indicador de finalización de calibración y estado de setpoint. */
const uint8_t ledCalibracion = 2;

/** @brief Pin del botón de inicio (SW par). */
const uint8_t BTN_RUN = 19;

/** @brief Pin del botón de parada (SW impar). */
const uint8_t BTN_STOP = 22;

/** @brief Pin reservado para el sensor infrarrojo (actualmente deshabilitado). */
//const uint8_t IR_PIN = 4;

// ============================
// CONFIGURACIÓN DE MOTORES
// ============================ 
/** @name Pines Motor Izquierdo */
///@{
const uint8_t motorPinIN1_Izq = 21;    ///< Control de dirección IN1.
const uint8_t motorPinIN2_Izq = 18;    ///< Control de dirección IN2.
const uint8_t motorPinSleep_Izq = 23;  ///< Pin de habilitación (Sleep).
///@}

/** @name Pines Motor Derecho */
///@{
const uint8_t motorPinIN1_Der = 26;    ///< Control de dirección IN1.
const uint8_t motorPinIN2_Der = 25;    ///< Control de dirección IN2.
const uint8_t motorPinSleep_Der = 16;  ///< Pin de habilitación (Sleep).
///@}

// ============================
// CONFIGURACIÓN SENSORES QTR & LINEA
// ============================
/** @name Asignación de Pines Array QTR */
///@{
const uint8_t S8 = 14;  ///< Sensor 8.
const uint8_t S7 = 27;  ///< Sensor 7.
const uint8_t S6 = 33;  ///< Sensor 6.
const uint8_t S5 = 32;  ///< Sensor 5.
const uint8_t S4 = 35;  ///< Sensor 4.
const uint8_t S3 = 34;  ///< Sensor 3.
const uint8_t S2 = 39;  ///< Sensor 2.
const uint8_t S1 = 36;  ///< Sensor 1.
///@}