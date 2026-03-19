/**
 @file buzzer.hpp
 @brief Implementación de la clase Buzzer para la generación de tonos y melodías PWM. Incluye las constantes de frecuencia para las notas musicales estándar.
 @author Legion de Ohm
 */

#pragma once
#include <Arduino.h>

#ifndef PITCHES_H
#define PITCHES_H

/**
 @name Frecuencias de la Octava 4 (Graves)
 @brief Definiciones de las notas fundamentales en la cuarta octava (262 Hz a 494 Hz).
 @{
 */
const uint16_t NOTE_C4 = 262;    ///< DO (262 Hz)
const uint16_t NOTE_D4 = 294;    ///< RE (294 Hz)
const uint16_t NOTE_E4 = 330;    ///< MI (330 Hz)
const uint16_t NOTE_F4 = 349;    ///< FA (349 Hz)
const uint16_t NOTE_G4 = 392;    ///< SOL (392 Hz)
const uint16_t NOTE_A4 = 440;    ///< LA (440 Hz) - Frecuencia de referencia
const uint16_t NOTE_B4 = 494;    ///< SI (494 Hz)
///@}

/**
 @name Frecuencias de la Octava 5 (Agudas)
 @brief Definiciones de las notas fundamentales en la quinta octava (523 Hz a 988 Hz).
 @{
 */
const uint16_t NOTE_C5 = 523;    ///< DO (523 Hz)
const uint16_t NOTE_D5 = 587;    ///< RE (587 Hz)
const uint16_t NOTE_E5 = 659;    ///< MI (659 Hz)
const uint16_t NOTE_F5 = 698;    ///< FA (698 Hz)
const uint16_t NOTE_G5 = 784;    ///< SOL (784 Hz)
const uint16_t NOTE_A5 = 880;    ///< LA (880 Hz)
const uint16_t NOTE_B5 = 988;    ///< SI (988 Hz)
///@}
#endif


#ifndef BUZZER_H
#define BUZZER_H
/**
 @class Buzzer
 @brief Clase para controlar el zumbador (buzzer) mediante la generación de tonos por PWM. Utiliza las funciones `ledc` del ESP32 para generar tonos precisos sin bloquear el `delay()`.
 */
class Buzzer {
  private:
    uint8_t pin;       ///< Pin GPIO al que está conectado el zumbador.
    uint8_t channel;   ///< Canal PWM (0-15) del ESP32 utilizado para generar la señal.

  public:
    /**
     @brief Constructor de la clase Buzzer.
     @param buzzerPin Pin GPIO al que está conectado el zumbador.
     @param pwmChannel Canal ledc del ESP32 a utilizar.
     */
    Buzzer(uint8_t buzzerPin, uint8_t pwmChannel);

    /**
     @brief Inicializa el periférico PWM (ledc) y lo asocia al pin del buzzer.
     @param freq Frecuencia base inicial del PWM en Hz (por defecto: 2000).
     @param resolution Resolución del duty cycle en bits (por defecto: 8).
     @return void
     */
    void begin(uint16_t freq = 2000, uint8_t resolution = 8);

    /**
     @brief Genera un tono con una frecuencia específica.
     @param freq Frecuencia de la nota en Hercios (Hz). Pasar 0 para silenciar.
     @return void
     */
    void play(uint16_t freq);

    /**
     @brief Detiene la generación de señal PWM, silenciando el zumbador.
     @return void
     */
    void stop();
};

/**
 @brief Instancia global de la clase Buzzer para ser utilizada en todo el proyecto.
 */
extern Buzzer buzzer; 

/**
 @brief Función de conveniencia para inicializar la instancia global del buzzer.
 @param freq Frecuencia base inicial (por defecto: 2000 Hz).
 @param resolution Resolución del PWM (por defecto: 8 bits).
 @return void
 */
void setupBuzzer(uint16_t freq = 2000, uint8_t resolution = 8);

#endif
