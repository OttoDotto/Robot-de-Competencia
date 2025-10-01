#ifndef PIDCONTROLLER_HPP
#define PIDCONTROLLER_HPP

class PIDController {
  public:
    PIDController(float kp = 0.0, float ki = 0.0, float kd = 0.0);
    void setTunings(float kp, float ki, float kd);
    float compute(float error);
    void reset();
    
  private:
    float kp, ki, kd;
    long lastError;
    long integral;
};

#endif
