/**
 @file config.hpp
 @brief Archivo de configuración global que define las constantes, banderas de compilación y la selección de perfil de corredor para el sistema PID.
 @author Legion de Ohm
 */
#pragma once
#include <Arduino.h>
#include "sensores.hpp"

// ============================
// PINES - LEDS, BOTONES, BOCINA y SENSOR IR
// ============================
/**
 @name Pines de Interfaz de Usuario
 @brief Definición de pines para periféricos de salida y botones de entrada.
 @{
 */
extern const uint8_t pinBuzzer;       ///< Pin GPIO asignado al zumbador.
extern const uint8_t ledMotores;      ///< LED indicador de estado de los motores.
extern const uint8_t ledCalibracion;  ///< LED indicador durante el proceso de calibración.
extern const uint8_t BTN_RUN;         ///< Pin del botón de inicio de carrera.
extern const uint8_t BTN_STOP;        ///< Pin del botón de parada de emergencia.
///@}

// ============================
// CONFIGURACIÓN DE MOTORES
// ============================
/**
 @name Configuración de Motores (Puente H)
 @brief Pines de control para los drivers de motor izquierdo y derecho.
 @{
 */
extern const uint8_t motorPinIN1_Izq;    ///< Entrada 1 del motor izquierdo.
extern const uint8_t motorPinIN2_Izq;    ///< Entrada 2 del motor izquierdo.
extern const uint8_t motorPinSleep_Izq;  ///< Pin de activación/reposo del motor izquierdo.

extern const uint8_t motorPinIN1_Der;    ///< Entrada 1 del motor derecho.
extern const uint8_t motorPinIN2_Der;    ///< Entrada 2 del motor derecho.
extern const uint8_t motorPinSleep_Der;  ///< Pin de activación/reposo del motor derecho.
///@}

// ============================
// CONFIGURACIÓN SENSORES QTR & LINEA
// ============================
/**
 @name Configuración Sensore QTR-8A
 @brief Pines analógicos asignados a la barra de sensores (S1 a S8).
 @{
 */
extern const uint8_t S8; ///< Pin sensor 8 (Extremo).
extern const uint8_t S7; ///< Pin sensor 7.
extern const uint8_t S6; ///< Pin sensor 6.
extern const uint8_t S5; ///< Pin sensor 5 (Centro).
extern const uint8_t S4; ///< Pin sensor 4 (Centro).
extern const uint8_t S3; ///< Pin sensor 3.
extern const uint8_t S2; ///< Pin sensor 2.
extern const uint8_t S1; ///< Pin sensor 1 (Extremo).
///@}


// ===================================
// MODO DEBUG - CAMBIAR EN PLATFORMIO.INI
// ===================================
/**
 @def DEBUG
 @brief Habilita los mensajes de depuración en el puerto serial. Macro que ejecuta el código 'x' si DEBUG está definido. Macro vacía (no hace nada) si DEBUG no está definido. Se activa añadiendo `-D DEBUG` en `platformio.ini`.
 */
#ifdef DEBUG
  #define deb(x) x
#else
  #define deb(x)
#endif


// ===================================
// SELECCION DE CORREDOR - CAMBIAR EN PLATFORMIO.INI
// ===================================
/**
 @name Perfiles de Corredor
 @brief Identificadores numéricos para cada perfil de constantes PID y calibración.
 */
///@{
/** 
 @def NIGHTFALL
 @brief Perfil de constantes NIGHTFALL. Valor: 1.
*/
#define NIGHTFALL 1
/** 
 @def ARGENTUM
 @brief Perfil de constantes ARGENTUM. Valor: 2.
*/
#define ARGENTUM  2
/** 
 @def DIEGO
 @brief Perfil de constantes DIEGO. Valor: 3.
*/
#define DIEGO     3
/**
 @def CORREDOR
 @brief La macro definida externamente (-D CORREDOR=...) que selecciona el perfil activo. Su valor debe ser uno de los identificadores numéricos definidos arriba (1, 2 o 3).
 @attention La compilación fallará si no se define en `platformio.ini` un valor válido para CORREDOR.
 */
#ifndef CORREDOR
  #error "Debe definir CORREDOR en platformio.ini (ej: -D CORREDOR=NIGHTFALL)"
#endif
///@}


// ===================================
// MODO MUTEADO - CAMBIA CUANDO SE HACE O NO PRUEBA
// ===================================
/**
 @def MUTEAR
 @brief Bandera de compilación para silenciar (mutear) todas las salidas de audio (buzzer). Se activa añadiendo `-D MUTEAR` en `platformio.ini`.
 */
#ifdef MUTEAR
    /** 
     @def mute(bz)
     @brief Macro que no ejecuta código si MUTEAR está definido.
    */
    #define mute(bz)
#else
    /** 
     @def mute(bz)
     @brief Macro que ejecuta la función de sonido `bz` si MUTEAR está definido.
    */    
   #define mute(bz) bz
#endif


// ===================================
// USAR CONTROL IR - CAMBIA CUANDO LO USAMOS Y CUANDO NO
// ===================================
/**
 @def USAR_CONTROL_IR
 @brief Bandera de compilación para habilitar la lógica de control remoto IR. Si no está definido, todo el código relacionado con la recepción IR se ignora.
*/
#ifdef USAR_CONTROL_IR
    /** 
     @def control_ir(IR)
     @brief Macro que ejecuta el código 'IR' si USAR_CONTROL_IR está definido.
    */
    #define control_ir(IR) IR
#else
    /**
     @def control_ir(IR)
     @brief Macro que no ejecuta código si USAR_CONTROL_IR no está definido.
    */
    #define control_ir(IR)
#endif