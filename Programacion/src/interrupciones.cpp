/**
 @file interrupciones.cpp
 @brief Implementación de la gestión de interrupciones externas y de temporizador.
 @details Configura las ISR para los botones de control y el temporizador de hardware 
 para garantizar un tiempo de muestreo constante en el lazo de control.
 @author Legion de Ohm
 */

#include "interrupciones.hpp"
#include "config.hpp"

// ============================
// FLAGS
// ============================
/** @brief Bandera volátil que indica si el sistema está en ejecución. */
volatile bool RUN = false;

/** @brief Bandera volátil para la gestión del estado de setpoint. */
volatile bool SETPOINT = true;

/** @brief Bandera volátil que señaliza el vencimiento del periodo del temporizador. */
volatile bool has_expired = false;

// ============================
// TIMER
// ============================
/** @brief Puntero al objeto del temporizador de hardware del ESP32. */
static hw_timer_t *timer = NULL;

/** @brief Periodo del temporizador en microsegundos (6000 us = 6ms). */
const int32_t TIEMPO_TIMER = 6000;              

/** @brief Tiempo diferencial fijo calculado en segundos para el PID. */
const float FIXED_DT_S = TIEMPO_TIMER * 1e-6f;  

// ============================
// ISR TIMER
// ============================
/**
 @brief ISR del Timer.
 @details Solo señaliza que el tiempo ha expirado activando la bandera has_expired. 
 Se ejecuta en IRAM para minimizar latencias.
 */
void IRAM_ATTR timerInterrupcion() {
    has_expired = true;
}

// ============================
// ISR BOTONES
// ============================
/**
 @brief ISR asociada al botón RUN.
 @details Activa las banderas RUN y SETPOINT para iniciar la lógica de movimiento.
 */
void IRAM_ATTR handleRun() {
    RUN = true;
    SETPOINT = true;
}

/**
 @brief ISR asociada al botón STOP.
 @details Desactiva la bandera RUN para detener inmediatamente la ejecución.
 */
void IRAM_ATTR handleStop() {
    RUN = false;
}

// ============================
// SETUP INTERRUPCIONES
// ============================
/**
 @brief Configura el hardware de interrupciones y el temporizador.
 @details Asocia los pines de los botones a sus ISR correspondientes y 
 configura el Timer 0 del ESP32 con un prescaler de 80 para contar en microsegundos.
 */
void setupInterrupciones() {
    // Interrupciones FISICAS de arranque y parada
    attachInterrupt(digitalPinToInterrupt(BTN_RUN), handleRun, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_STOP), handleStop, RISING);

    // Interrupciones por TIMER - Creamos el Timer 0, dividimos los 80MHz en 80 - cada 1us
    timer = timerBegin(0, 80, true);

    // Asocia la funcion de interrupcion (ISR)
    timerAttachInterrupt(timer, &timerInterrupcion, true);
    
    // Configura cada cuánto se activa la interrupcion
    timerAlarmWrite(timer, TIEMPO_TIMER, true);
   
    // Enciende el timer para que empiece a generar las interrupciones
    timerAlarmEnable(timer);
}
