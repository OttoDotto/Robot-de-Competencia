/**
 @file interrupciones.hpp
 @brief Cabecera para la gestión de las interrupciones externas (ISR). Este módulo maneja la lógica de inicio y detención de la rutina principal del robot a través de un pin de interrupción (ej. un botón o jumper de seguridad).
 @author Legion de Ohm
 */

#pragma once
#include <Arduino.h>

// ============================
// FLAGS COMPARTIDOS
// ============================
/**
 @var RUN
 @brief Bandera de estado que controla si el ciclo principal del robot debe ejecutarse. El calificador `volatile` es crucial para indicar que esta variable puede ser modificada por una función de interrupción (ISR) en cualquier momento y debe ser leída directamente de la memoria en el ciclo principal (`loop`).
 */
extern volatile bool RUN;

/**
 @var SETPOINT
 @brief Bandera volátil que indica si el robot ha alcanzado o debe buscar el punto de consigna (setpoint).
 */
extern volatile bool SETPOINT;

/**
 @var has_expired
 @brief Bandera de sincronización utilizada por el temporizador para indicar que un periodo de tiempo ha finalizado.
 */
extern volatile bool has_expired;

// ============================
// TIEMPO DE CONTROL
// ============================
/**
 @brief Periodo del temporizador de control expresado en microsegundos (us).
 */
extern const int32_t TIEMPO_TIMER;

/**
 @brief Tiempo diferencial fijo (Delta T) expresado en segundos (s) para los cálculos del PID.
 */
extern const float FIXED_DT_S;

// ============================
// INICIALIZACIÓN
// ============================
void setupInterrupciones();

// ============================
// ISRs
// ============================
/**
 @brief Función de servicio de interrupción (ISR) para iniciar la rutina principal. El atributo `IRAM_ATTR` se utiliza en el ESP32 para colocar la función en la RAM interna, asegurando que se pueda ejecutar rápidamente sin problemas de caché o latencia, lo cual es vital para las interrupciones. Esta función probablemente establece `RUN = true;`.
 */
void IRAM_ATTR handleRun();

/**
 @brief Función de servicio de interrupción (ISR) para detener la rutina principal. Se ejecuta cuando se detecta el evento de detención en el pin de interrupción. Esta función probablemente establece `RUN = false;`.
 */
void IRAM_ATTR handleStop();

/**
 @brief ISR del temporizador de hardware. 
 @details Se ejecuta periódicamente para gestionar tareas de tiempo real, como el refresco del cálculo PID. Utiliza `IRAM_ATTR` para máxima velocidad de respuesta.
 */
void IRAM_ATTR timerInterrupcion();
