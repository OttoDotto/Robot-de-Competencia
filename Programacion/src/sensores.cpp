/**
 @file sensores.cpp
 @brief Implementación de la lectura y configuración de los sensores QTR-8A.
 @details Maneja el objeto de la librería QTRSensors, el proceso de calibración inicial 
 y la lógica de detección de posición según el color de la línea de competencia.
 @author Legion de Ohm
 */

#include "sensores.hpp"
#include "QTRSensors.h"
#include "config.hpp"
#include "motores.hpp"
#include "buzzer.hpp"

// ============================
// CONFIGURACIÓN QTR
// ============================

/** @brief Objeto estático para manejar los sensores QTR. */
static QTRSensors qtr;

/** @brief Número total de sensores configurados (8 en total). */
static const uint8_t SensorCount = 8;

/** @brief Array que mapea los pines físicos S1-S8 definidos en config.hpp. */
static const uint8_t sensorPins[SensorCount] = {S8, S7, S6, S5, S4, S3, S2, S1};

/** @brief Array para almacenar los valores brutos de lectura de cada sensor. */
static uint16_t sensorValues[SensorCount];

/** @brief Variable global que almacena la última posición calculada de la línea. */
uint16_t position;

// ===================================
// LINEA NEGRA o BLANCA - CAMBIAR EN PLATFORMIO.INI
// ===================================

#ifdef LINEA_NEGRA
    /** @brief Configuración global para pista con línea negra sobre fondo blanco. */
    Linea linea_competencia = NEGRA;
#else
    /** @brief Configuración global para pista con línea blanca sobre fondo negro. */
    Linea linea_competencia = BLANCA;
#endif 

// ============================
// SETUP DE SENSORES
// ============================

/**
 @brief Configura el tipo de sensor y los pines asociados.
 @details Establece el modo de lectura analógica y llama a la rutina de calibración.
 */
void setupSensores() {
    // Usaremos lectura analógica (ADC)
    qtr.setTypeAnalog();
    
    // Cargamos los pines de sensores QTR
    qtr.setSensorPins(sensorPins, SensorCount);

    // Calibracion inicial
    calibrarSensores();
}

// ============================
// FUNCION CALIBRAR
// ============================

/**
 @brief Ejecuta la rutina de calibración de los sensores.
 @details Realiza 300 muestras de lectura para determinar los umbrales de blanco y negro. 
 Proporciona feedback sonoro (buzzer) y visual (LED) durante el proceso.
 */
void calibrarSensores() {
    // Aviso sonoro de inicio de calibración (solo si mute está habilitado)
    mute(
        buzzer.play(NOTE_A4);
        delay(150);
        buzzer.stop();
    );

    // Encendemos el LED de estado para indicar proceso de calibración
    digitalWrite(ledCalibracion, HIGH);
    deb(Serial.println("Calibrando sensores..."); )

    // Se realizan múltiples lecturas para que la librería QTR tome
    // valores mínimo y máximo de cada sensor y ajuste su calibración
    for (uint16_t i = 0; i < 300; i++) { qtr.calibrate(); }    

    // Apagamos indicador de calibración
    digitalWrite(ledCalibracion, LOW);
    deb(Serial.println("Calibracion lista!");)

    // Aviso de final de calibración (solo si mute está habilitado)
    mute(
        buzzer.play(NOTE_C5);
        delay(200);
        buzzer.stop();
    )
}


// ============================
// LECTURA DE POSICIÓN
// ============================

/**
 @brief Obtiene la posición relativa del robot respecto a la línea.
 @details Selecciona entre readLineWhite o readLineBlack según la variable linea_competencia.
 @return uint16_t Valor normalizado entre 0 y 7000.
 */
uint16_t leerLinea() {
    // Dependiendo del color de la pista, se usa lectura inversa:
    if (linea_competencia == BLANCA)    position = qtr.readLineWhite(sensorValues);
    else                                position = qtr.readLineBlack(sensorValues);

    // Devuelve un valor entre ~0 (izquierda) y ~7000 (derecha)
    return position;
}