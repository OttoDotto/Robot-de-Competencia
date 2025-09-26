#include <Arduino.h>

// Pines conectados al DRV8833 (cambiá si usás otros)
#define IN1_M1 14
#define IN2_M1 27
#define IN1_M2 33
#define IN2_M2 32

// Canales PWM asignados (ESP32 tiene 16 disponibles)
#define CH_IN1_M1 0
#define CH_IN2_M1 1
#define CH_IN1_M2 2
#define CH_IN2_M2 3

// Parámetros PWM
#define PWM_FREQ 1000       // Frecuencia en Hz
#define PWM_RES 8           // Resolución de 8 bits (0–255)
#define PWM_DUTY 200        // Potencia aplicada

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Probando motores (ESP32 + DRV8833)");

  // Asignar pines a canales PWM
  ledcAttachPin(IN1_M1, CH_IN1_M1);
  ledcAttachPin(IN2_M1, CH_IN2_M1);
  ledcAttachPin(IN1_M2, CH_IN1_M2);
  ledcAttachPin(IN2_M2, CH_IN2_M2);

  // Configurar canales PWM
  ledcSetup(CH_IN1_M1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN2_M1, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN1_M2, PWM_FREQ, PWM_RES);
  ledcSetup(CH_IN2_M2, PWM_FREQ, PWM_RES);
}

void loop() {
  // Motor 1 adelante
  Serial.println("Motor 1 adelante");
  ledcWrite(CH_IN1_M1, PWM_DUTY);
  ledcWrite(CH_IN2_M1, 0);
  delay(1000);

  // Motor 1 atrás
  Serial.println("Motor 1 atrás");
  ledcWrite(CH_IN1_M1, 0);
  ledcWrite(CH_IN2_M1, PWM_DUTY);
  delay(1000);

  // Freno Motor 1
  Serial.println("Motor 1 freno");
  ledcWrite(CH_IN1_M1, 0);
  ledcWrite(CH_IN2_M1, 0);
  delay(1000);

  // Motor 2 adelante
  Serial.println("Motor 2 adelante");
  ledcWrite(CH_IN1_M2, PWM_DUTY);
  ledcWrite(CH_IN2_M2, 0);
  delay(1000);

  // Motor 2 atrás
  Serial.println("Motor 2 atrás");
  ledcWrite(CH_IN1_M2, 0);
  ledcWrite(CH_IN2_M2, PWM_DUTY);
  delay(1000);

  // Freno Motor 2
  Serial.println("Motor 2 freno");
  ledcWrite(CH_IN1_M2, 0);
  ledcWrite(CH_IN2_M2, 0);
  delay(1000);
}

