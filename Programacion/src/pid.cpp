/**
 @file pid.cpp
 @brief Implementación del controlador Proporcional-Integral-Derivativo (PID).
 @details Gestiona la selección de constantes por perfil de corredor y realiza el cálculo
 de la salida del controlador basándose en el error de posición de la línea.
 @author Legion de Ohm
 */

#include <Arduino.h>
#include "config.hpp"
//#include "drv8833.hpp"
#include "pid.hpp"

/** @brief Almacena el error de la iteración anterior para el cálculo de la parte derivativa. */
float  lastError = 0;

/** @brief Acumulador del error en el tiempo para el cálculo de la parte integral. */
float  integral = 0;

/** @brief Almacena la marca de tiempo de la última ejecución (no utilizado en la versión actual). */
uint32_t lastTime = 0;


// ===================================
// SELECCION DE CORREDOR - CAMBIAR EN PLATFORMIO.INI
// ===================================
#if (CORREDOR == NIGHTFALL)
    uint8_t baseSpeed = 70;         ///< Velocidad crucero para NIGHTFALL (0-100%).
    const float Ku = 0.065;         ///< Ganancia última para NIGHTFALL.
    const float Tu = 0.350;         ///< Periodo último para NIGHTFALL.
#elif (CORREDOR == ARGENTUM)
    uint8_t baseSpeed = 78;         ///< Velocidad crucero para ARGENTUM (0-100%).
    const float Ku = 0.05;          ///< Ganancia última para ARGENTUM.
    const float Tu = 0.31;          ///< Periodo último para ARGENTUM.
#elif (CORREDOR == DIEGO)
    uint8_t baseSpeed = 70;         ///< Velocidad crucero para DIEGO (0-100%).
    const float Ku = 0.05;          ///< Ganancia última para DIEGO.
    const float Tu = 0.38;          ///< Periodo último para DIEGO.
#else
  #error "Valor de CORREDOR inválido. Use NIGHTFALL, ARGENTUM o DIEGO."
#endif


// ============================
// CONTROL PID - METODO Ziegler-Nichols
// ============================
#ifdef TEST_PID
    const float Kp = Ku;            ///< Configuración de prueba: Solo P.
    const float Ki = 0;             ///< Configuración de prueba: I desactivada.
    const float Kd = 0;             ///< Configuración de prueba: D desactivada.
#else
    /** @brief Ganancia Proporcional según sintonización clásica de Ziegler-Nichols (0.6 * Ku). */
    const float Kp = 0.6 * Ku;
    /** @brief Ganancia Integral calculada como @f$ 2 \cdot K_p / T_u @f$. */
    const float Ki = 2 * Kp / Tu;
    /** @brief Ganancia Derivativa calculada como @f$ K_p \cdot T_u / 8 @f$. */
    const float Kd = Kp * Tu / 8;
#endif


// ============================
// FUNCION CALCULO DE PID
// ============================
/**
 @brief Realiza el cálculo del algoritmo PID.
 @details Calcula el error respecto al setpoint, la tasa de cambio (derivada) y la 
 acumulación del error (integral) para generar la señal de salida.
 @param pos Posición actual leída por el array de sensores.
 @param deltaTime Tiempo transcurrido desde la última ejecución en segundos (@f$ \Delta T @f$).
 @return float Señal de corrección resultante de la suma ponderada de los tres términos.
 */
float calculo_pid(uint16_t pos, float deltaTime) {
    // Calcular deltaTime (comentado en código original)
    //float  deltaTime = (now - lastTime) / TIME_DIVISOR; 
    deb(Serial.printf("deltaTime=%.3f\n", deltaTime);)

    // Calcular el error
    float  error = pos - setpoint;               
    
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


/**
 @brief Reinicia las variables de estado del controlador.
 @details Pone a cero el error previo y el acumulador integral para evitar efectos 
 de arrastre en nuevas activaciones del control.
 */
void reiniciar_pid() {
    lastError = 0; 
    integral = 0;
}