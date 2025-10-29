/**
 * \file drv8833.hpp
 * \brief Header para el driver de motores drv8833
* \author Legion de Ohm
 */

#pragma once
/**
 * \brief Clase para drivers de motores Drv8833. Incluye funciones
 * que permiten configurar sus pines, ajustar el PWM y el movimiento de los motores.
 */
class Drv8833
{
public:
    /**
     * \brief Constructor inicial. Habilitamos GPIO Sleep, activando los motores.
     */
    Drv8833();

    /**
     * \brief Función: configurar los pines de control del motor y el PWM.
     * \param pinIN1: GPIO al que se conecta el pin IN1 del driver.
     * \param pinIN2: GPIO al que se conecta el pin IN2 del driver.
     * \param pinSleep: GPIO al que se conecta el pin Sleep del driver 
     * \param chPWM: Canal del PWM a utilizar con el driver.
     * \param freqPWM: Frecuencia del PWM. (ESP32: De 10 Hz a 40 MHz)
     * \param resPWM: Resolución del PWM. (ESP32: De 1 bit [0-1] a 16 bits [0-65536])
     */
    void setup(uint8_t pinIN1, uint8_t pinIN2, uint8_t pinSleep, uint8_t chPWM, uint32_t freqPWM, uint8_t resPWM);

    /**
     * \brief Función para mover el motor hacia adelante.
     * \param pPWM: Porcentaje de PWM  a aplicar al motor [0, 100] %.
     */
    void forward(uint8_t pPWM);

    /**
     * \brief Función para mover el motor hacia atras.
     * \param pPWM: Porcentaje de PWM  a aplicar al motor [0, 100] %.
     */
    void reverse(uint8_t pPWM);

    /**
     * \brief Función para detener el motor.
     * \param NA: Sin parámetros.
     */
    void stop();

private:
    uint8_t _pinIN1;   // GPIO del driver pin IN1.
    uint8_t _pinIN2;   // GPIO del driver pin IN2.
    uint8_t _pinSleep; // GPIO del driver pin Sleep
    uint8_t _chPWM;    // Canal del PWM a utilizar con el motor.
    uint32_t _freqPWM; // Frecuencia del PWM. (ESP32: De 10 Hz a 40 MHz)
    uint8_t _resPWM;   // Resolución del PWM. (ESP32: De 1 a 16 bits)
};