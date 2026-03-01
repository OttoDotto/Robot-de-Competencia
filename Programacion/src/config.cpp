#include "config.hpp"


// ===================================
// LINEA NEGRA o BLANCA - CAMBIAR EN PLATFORMIO.INI
// ===================================
#ifdef LINEA_NEGRA
    Linea linea_competencia = NEGRA;
#else
    Linea linea_competencia = BLANCA;
#endif 


// ===================================
// SELECCION DE CORREDOR - CAMBIAR EN PLATFORMIO.INI
// ===================================
#if (CORREDOR == NIGHTFALL)
    #define USAR_MICROS   1         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 55;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.05;
    const float Tu = 0.50;
#elif (CORREDOR == ARGENTUM)
    #define USAR_MICROS   0         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 55;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.051;
    const float Tu = 0.31;
#elif (CORREDOR == DIEGO)
    #define USAR_MICROS   0         // Cambiar a 0 para millis()
    uint8_t baseSpeed = 45;         // Velocidad base PORCENTAJE DE PWM (0-100%)
    const float Ku = 0.051;
    const float Tu = 0.31;
#else
  #error "Valor de CORREDOR inválido. Use NIGHTFALL, ARGENTUM o DIEGO."
#endif


// ============================
// CONTROL PID - METODO Ziegler-Nichols
// ============================
#ifdef TEST_PID
    const float Kp = Ku;
    const float Ki = 0;
    const float Kd = 0;
    #define MUTEAR
#else
    const float Kp = 0.6 * Ku;
    const float Ki = 2 * Kp / Tu;
    const float Kd = Kp * Tu / 8;
#endif