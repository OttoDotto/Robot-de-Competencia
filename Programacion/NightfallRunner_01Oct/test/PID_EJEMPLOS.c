void PID(){
  errorActual = setpoint - pos;
  derivativo = errorActual-last_prop;
  integral = error1+error2+error3+error4+error5+error6;
  last_prop = errorActual;
  error6 = error5;
  error5 = error4;
  error4 = error3;
  error3 = error2;
  error2 = error1;
  error1 = errorActual;
  int output=(errorActual*KP) + (derivativo*KD) + (integral*KI);
  if(output > vel) output=vel;
  else if(output < -vel) output=-vel;
  (output < 0)?
  motores(vel, vel+output):motores(vel-output, vel);
}

// Función PID
void PID() {
    // Obtener el tiempo actual y calcular deltaTime
    unsigned long now = millis();
    double deltaTime = (now - lastTime) / 1000.0; // Convertir a segundos
    lastTime = now;

    // Calcular el error
    double error = setpoint - pos;

    // Calcular derivativo (tasa de cambio del error)
    double derivativo = (error - last_prop) / deltaTime;

    // Calcular integral (acumulación del error)
    integralSum += error * deltaTime;

    // Actualizar el último error proporcional
    last_prop = error;

    // Calcular la salida del PID
    int output = (error * KP) + (derivativo * KD) + (integralSum * KI);

    // Limitar la salida para que no exceda los límites
    if (output > 255) output = 255; // Limitar máximo
    else if (output < -255) output = -255; // Limitar mínimo

    // Controlar los motores según la salida
    controlarMotores(output);

    // Imprimir valores para depuración
    Serial.print("Setpoint: "); Serial.print(setpoint);
    Serial.print(" | Pos: "); Serial.print(pos);
    Serial.print(" | Error: "); Serial.print(error);
    Serial.print(" | Output: "); Serial.println(output);
}



