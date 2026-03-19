#include <Arduino.h>
#include <IRremote.hpp>

// Pines
#define LED_EXTERNAL 13
#define BTN_ON  19
#define BTN_OFF 22
#define IR_PIN  4

// Control
#define CMD_ON   0x18
#define RAW_ON   0xE718FF00

#define CMD_OFF  0x1C
#define RAW_OFF  0xE31CFF00


#define USAR_CONTROL_IR
#ifdef USAR_CONTROL_IR
    // Usamos el control
    #define control_ir(IR) IR
#else
    // No usamos el control
    #define control_ir()
#endif

// Estado global del LED
volatile bool ledState = false;

// ISR físicas
void IRAM_ATTR handleOn()  { ledState = true;  }
void IRAM_ATTR handleOff() { ledState = false; }


void setup() {
  Serial.begin(115200);

  pinMode(LED_EXTERNAL, OUTPUT);
  pinMode(BTN_ON, INPUT);
  pinMode(BTN_OFF, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_ON), handleOn, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN_OFF), handleOff, RISING);

  // Inicializar receptor IR
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Sistema iniciado. Usa botones o control IR para encender/apagar el LED.");
}


void loop() {
  // --- Lectura IR ---
  control_ir(  
    if (IrReceiver.decode()) {
      auto data = IrReceiver.decodedIRData;
      uint32_t raw = data.decodedRawData;
      uint8_t cmd  = data.command;

      const char* proto = IrReceiver.getProtocolString();

      Serial.println("\n--- Señal IR recibida ---");
      Serial.printf("Protocolo: %s\n", proto);
      Serial.printf("Dirección: 0x%04X\n", data.address);
      Serial.printf("Comando  : 0x%02X\n", data.command);
      Serial.printf("Flags    : 0x%02X\n", data.flags);
      Serial.printf("RawData  : 0x%08lX\n", data.decodedRawData);
      Serial.printf("Bits     : %d\n", data.numberOfBits);
      Serial.printf("Repetido : %s\n", data.flags & IRDATA_FLAGS_IS_REPEAT ? "SI" : "NO");

      // Solo aceptar comandos si la dirección es la "autorizada"
      if (raw == RAW_ON && cmd == CMD_ON) {  // Botón ON
        ledState = true;
      }
      if (raw == RAW_OFF && cmd == CMD_OFF) {  // Botón OFF
        ledState = false;
      }
      IrReceiver.resume();
    }
  )

  // --- Actualización del LED ---
  digitalWrite(LED_EXTERNAL, ledState ? HIGH : LOW);
}