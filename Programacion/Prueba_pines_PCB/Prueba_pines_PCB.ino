/*
* PINES ESP32 ELEJIDOS:	(D = GPIO !!!!)
Sensores (fila bajo el USB, contiguos para pistas rectas)
S1 → GPIO36 (VP)
S2 → GPIO39 (VN)
S3 → GPIO34
S4 → GPIO35
S5 → GPIO32
S6 → GPIO33
S7 → GPIO27
S8 → GPIO14

Puente H izquierdo (M1) – lado izquierdo del ESP32
IN1_M1 → GPIO26 (PWM)
IN2_M1 → GPIO25 (PWM)
SLEEP_M1 → GPIO16

Puente H derecho (M2) – lado derecho del ESP32
IN1_M2 → GPIO21 (PWM)
IN2_M2 → GPIO18 (PWM)
SLEEP_M2 → GPIO23

Botones, buzzer, LED (abajo/derecha)
SW1 → GPIO22 (entrada con pull-up interno)
SW2 → GPIO19 (entrada con pull-up interno)
BUZZER → GPIO17 (PWM con LEDC)			(PROBLEMA, queda atrapado)
LED → GPIO13 (salida)
*/

// Sensores (solo lectura, ADC)
int sensores[] = {36, 39, 34, 35, 32, 33, 27, 14};
int totalSensores = sizeof(sensores) / sizeof(sensores[0]);

// Motores (IN1, IN2, SLEEP)
int motor1[] = {26, 25, 16}; // IN1, IN2, SLEEP
int motor2[] = {21, 18, 23}; // IN1, IN2, SLEEP

// Botones
int botones[] = {22, 19};
int totalBotones = sizeof(botones) / sizeof(botones[0]);

// Periféricos simples
int BUZZER = 17;
int LED = 13;

void setup() {
  Serial.begin(115200);
  Serial.println("Test PCB - ESP32 (arrays)");

  // Sensores
  for (int i = 0; i < totalSensores; i++) {
    pinMode(sensores[i], INPUT);
  }

  // Motores
  for (int i = 0; i < 3; i++) {
    pinMode(motor1[i], OUTPUT);
    pinMode(motor2[i], OUTPUT);
  }

  // Botones
  for (int i = 0; i < totalBotones; i++) {
    pinMode(botones[i], INPUT_PULLUP);
  }

  // Periféricos
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Sensores
  Serial.print("Sensores: ");
  for (int i = 0; i < totalSensores; i++) {
    Serial.print(analogRead(sensores[i]));
    Serial.print(" ");
  }
  Serial.println();

  // Botones
  Serial.print("Botones: ");
  for (int i = 0; i < totalBotones; i++) {
    Serial.print(digitalRead(botones[i]));
    Serial.print(" ");
  }
  Serial.println();

  // Motores: cambiar estado simple
  digitalWrite(motor1[2], HIGH); // SLEEP_M1
  digitalWrite(motor2[2], HIGH); // SLEEP_M2

  digitalWrite(motor1[0], HIGH); digitalWrite(motor1[1], LOW);
  digitalWrite(motor2[0], HIGH); digitalWrite(motor2[1], LOW);
  delay(300);
  digitalWrite(motor1[0], LOW); digitalWrite(motor1[1], HIGH);
  digitalWrite(motor2[0], LOW); digitalWrite(motor2[1], HIGH);
  delay(300);

  // LED y buzzer
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
  delay(100);
}

