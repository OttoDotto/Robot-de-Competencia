#include <Arduino.h>
#include "drv8833.hpp"

/*
  PROGRAMA PARA PRUEBA DE MOTORES
*/

// MOTOR IZQUIERDO
// Pines
const uint8_t motorPinIN1_Izq = 18;
const uint8_t motorPinIN2_Izq = 21;
const uint8_t motorPinSleep_Izq = 23;

// MOTOR DERECHO
// Pines
const uint8_t motorPinIN1_Der = 26;
const uint8_t motorPinIN2_Der = 25;
const uint8_t motorPinSleep_Der = 16;

// Canales PWM
const uint8_t motorPWM = 0; // Canal PWM 0
// Ajustes PWM
const uint32_t freqPWM = 10000; // Frecuencia del PWM = 10KHz
const uint8_t resPWM = 8;       // Resolución de 8 bits = 256 valores posibles [0, 255]

// Creación de dos objetos de la clase Motor
Drv8833 motorDer;
Drv8833 motorIzq;

void setup()
{
  // Ajustes iniciales del motor
  motorDer.setup(motorPinIN1_Der, motorPinIN2_Der, motorPinSleep_Der, motorPWM, freqPWM, resPWM);
  motorIzq.setup(motorPinIN1_Izq, motorPinIN2_Izq, motorPinSleep_Izq, motorPWM, freqPWM, resPWM);

}

void loop()
{
  // Mover los motores hacia adelante al 50% del PWM
  // Menor a 40% apenas se mueve                    [TODO:  Revisar por que pasa eso...]
  motorIzq.forward(50);
  motorDer.forward(50);
  delay(2000);

  // Detener motores
  motorIzq.stop();
  motorDer.stop();
  delay(20);

  // Mover los motores  hacia atras al 100% del PWM
  motorIzq.reverse(100);
  motorDer.reverse(100);
  delay(2000);

  // Detener motores
  motorIzq.stop();
  motorDer.stop();
  delay(1000);
}