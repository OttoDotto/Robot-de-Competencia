# Biblioteca Driver de motores DRV8833 para ESP32

## Configuración
La configuración de motor se realiza mediante la función ``setup`` que contiene los siguientes parámetros:
- **pinIN1**: GPIO al que se conecta el pin IN1 del motor.
- **pinIN2**: GPIO al que se conecta el pin IN2 del motor.
- **chPWM**: Canal del PWM a utilizar con el motor. (ESP32: 16 Canales)
- **freqPWM**: Frecuencia del PWM. (ESP32: De 10 Hz a 40 MHz)
- **resPWM**: Resolución del PWM. (ESP32: De 1 bit a 16 bits)

## Movimientos
Para mover el motor se hace uso de las siguientes funciones:
- Función ``forward``: Mover el motor hacia adelante con un porcentaje de PWM dado [0,100]%.
- Función ``reverse``: Mover el motor hacia atrás con un porcentaje de PWM dado [0,100]%.
- Función ``stop``: Detener el motor.