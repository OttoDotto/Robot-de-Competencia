#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, 5000, 8);  // 5 kHz, resolución 8 bits (0-255)
}

void loop() {
  // Aumenta brillo
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_PIN, duty);
    delay(10);
  }

  // Disminuye brillo
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_PIN, duty);
    delay(10);
  }
}
