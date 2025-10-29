#include <Arduino.h>
#include <IRremote.hpp>

#define IR_PIN 4      // GPIO del receptor VS1838B
#define LED_PIN 13    // GPIO del LED indicador
#define LED_BUILTIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Inicializa el receptor IR
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Receptor IR listo. Apunta un control remoto...");
  Serial.println("-------------------------------------------------");
}

void loop() {
  if (IrReceiver.decode()) {

    // Enciende el LED para indicar recepción
    digitalWrite(LED_PIN, HIGH);

    // Extraer datos del paquete recibido
    const uint8_t command = IrReceiver.decodedIRData.command;
    const auto protocol = IrReceiver.getProtocolString();
    const uint32_t rawData = IrReceiver.decodedIRData.decodedRawData;

    // Mostrar información principal
    Serial.println("=== Señal IR detectada ===");
    Serial.print("Protocolo: ");
    Serial.println(protocol);
    Serial.print("Comando (Command): 0x");
    Serial.println(command, HEX);
    Serial.print("Raw Data: 0x");
    Serial.println(rawData, HEX);
    Serial.println("---------------------------");

    // Pequeña pausa visual del LED
    delay(80);
    digitalWrite(LED_PIN, LOW);

    // Preparar para la siguiente lectura
    IrReceiver.resume();
  }
}
