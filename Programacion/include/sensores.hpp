/**
 @file sensores.hpp
 @brief Cabecera para la configuración, calibración y lectura del array de sensores QTR. Contiene la definición del tipo de línea a seguir, la instancia del objeto QTR y las funciones para obtener la posición central de la línea.
 @author Legion de Ohm
 */

#pragma once
#include <Arduino.h>

// ===================================
// TIPOS DE DATOS Y CONSTANTES
// ===================================
/**
 @enum Linea
 @brief Define el modo de operación del seguidor de línea: Indica si el robot debe buscar la línea oscura (NEGRA) o la línea clara (BLANCA). Este valor se configura globalmente en `config.cpp` mediante la bandera `LINEA_NEGRA`.
 */
enum Linea { 
    BLANCA, ///< El robot sigue la línea blanca sobre fondo negro (valores altos = sobre línea).
    NEGRA   ///< El robot sigue la línea negra sobre fondo blanco (valores bajos = sobre línea).
};

/**
 @var position
 @brief Posición calculada de la línea. Es el valor de la salida del proceso (PV), usado como entrada para el controlador PID. Este valor se normaliza (ej. 0 a 4000) donde el centro es el punto de referencia (`setpoint`).
 */
extern uint16_t position;

// ===================================
// PROTOTIPOS DE FUNCIONES
// ===================================
/**
 @brief Inicializa el objeto QTRSensors con los pines de entrada/salida.
 @param sensorPins Array que contiene los pines GPIO a los que están conectados los sensores.
 @param SensorCount Número total de sensores a inicializar.
 @return void
 */
void setupSensores();

/**
 @brief Rutina de calibración de los sensores para adaptarse a las condiciones de la pista. Ejecuta un ciclo de calibración (ej. 2.5 segundos) para determinar los valores mínimo y máximo de blanco y negro, crucial para la lectura precisa de la línea.
 @return void
 */
void calibrarSensores();

/**
 @brief Lee los sensores y calcula la posición ponderada de la línea. Utiliza la función `readLine()` de la librería QTR para devolver un valor normalizado que indica el desplazamiento lateral respecto al centro del array.
 @return uint16_t Posición calculada de la línea (ej. 0 a 4000).
 */
uint16_t leerLinea();

