/**
 @file buzzer.cpp
 @brief Implementación de los métodos de la clase Buzzer y funciones de control de audio.
 @author Legion de Ohm
 */

#include <Arduino.h>
#include "buzzer.hpp"
#include "config.hpp"

/**
 @brief Constructor de la clase Buzzer.
 @param buzzerPin Pin GPIO asignado al hardware del zumbador.
 @param pwmChannel Canal del periférico ledc a utilizar.
 */
Buzzer::Buzzer(uint8_t buzzerPin, uint8_t pwmChannel) {
  pin = buzzerPin;
  channel = pwmChannel;
}

/**
 @brief Configura el periférico PWM para el zumbador.
 @param freq Frecuencia inicial de la señal.
 @param resolution Resolución en bits para el control del duty cycle.
 */
void Buzzer::begin(uint16_t freq, uint8_t resolution) {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(pin, channel);
}

/**
 @brief Genera un tono continuo en el canal configurado.
 @param freq Frecuencia del tono en Hercios (Hz).
 */
void Buzzer::play(uint16_t freq) {
  ledcWriteTone(channel, freq);
}

/**
 @brief Detiene la generación de tono enviando una frecuencia de 0.
 */
void Buzzer::stop() {
  ledcWriteTone(channel, 0);
}

/**
 @brief Instancia global del objeto Buzzer.
 @details Se inicializa utilizando el pin definido en la configuración y el canal 2 del ESP32.
 */
Buzzer buzzer(pinBuzzer, 2);

/**
 @brief Función global para inicializar la instancia 'buzzer'.
 @param freq Frecuencia base de inicialización.
 @param resolution Resolución del PWM.
 */
void setupBuzzer(uint16_t freq, uint8_t resolution ) {
    buzzer.begin(freq , resolution);
}