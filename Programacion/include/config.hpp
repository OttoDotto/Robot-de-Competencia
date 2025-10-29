#pragma once
#include <Arduino.h>
#include "sensores.hpp"

extern Linea linea_competencia;

extern uint8_t baseSpeed;

// ============================
// CONTROL PID - METODO Ziegler-Nichols
// ============================
extern const float Ku;
extern const float Tu;

extern const float Kp;
extern const float Ki;
extern const float Kd;


// ===================================
// MODO DEBUG - CAMBIAR EN PLATFORMIO.INI
// ===================================
#ifdef DEBUG
    // Hacemos lo que esta dentro
    #define deb(x) x
#else
    // No hacemos lo que esta dentro
    #define deb(x)
#endif


// =================================
// CONFIGURACIÓN DE TIEMPOS - CAMBIA SEGUN EL CORREDOR 
// =================================
#if USAR_MICROS
  #define tiempoActual() micros()
  const float TIME_DIVISOR = 1000000.0; // microsegundos → segundos
#else
  #define tiempoActual() millis()
  const float TIME_DIVISOR = 1000.0;    // milisegundos → segundos
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


// ===================================
// MODO MUTEADO - CAMBIA CUANDO SE HACE O NO PRUEBA
// ===================================
#ifdef MUTEAR
    // No sonamos la bocina
    #define mute(bz)
#else
    // Sonamos la bocina
    #define mute(bz) bz
#endif