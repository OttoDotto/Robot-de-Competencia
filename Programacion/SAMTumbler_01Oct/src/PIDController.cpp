#include "PIDController.hpp"
#include <Arduino.h>

PIDController::PIDController(float kp, float ki, float kd)
: kp(kp), ki(ki), kd(kd), lastError(0), integral(0) {}

void PIDController::setTunings(float kp, float ki, float kd) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
}

float PIDController::compute(float error) {
  float P = kp * error;
  integral += error;
  integral = constrain(integral, -10000, 10000);
  float I = ki * integral;
  float D = kd * (error - lastError);

  lastError = error;
  return P + I + D;
}

void PIDController::reset() {
  integral = 0;
  lastError = 0;
}
