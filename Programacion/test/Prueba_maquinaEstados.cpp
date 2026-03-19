#include <Arduino.h>

// ===================== LEDs =====================
const int LED_INTERNAL = 2, LED_EXTERNAL = 13;

// ===================== Botones (pull-down) =====================
const int BTN_ON  = 19; // RUN toggle
const int BTN_SP  = 22; // SP toggle

// ===================== Señales de control =====================
volatile bool RUN = 0, SP  = 1;

// ===================== Prototipos estados  =====================
void estadoStop();
void estadoAcel();
void estadoControl();

// Punteros a funciones de estado
void (*acciones_estado[])() = { estadoStop, estadoAcel, estadoControl };

// ===================== ISR =====================
// MODO CORREDOR
void IRAM_ATTR handleRun() {
    RUN = true;
    SP = true;
}

// MODO CONTROL 
void IRAM_ATTR handleControl(){
    SP = false;
}

enum estadoStop { CUALQUIERA = -1, S, A, C, CANT_estadoStop };

// función dummy
int nada(int c) { return 0; }

typedef struct {
    int recibo;               // combinación SP,RUN (00..11)
    int (*transicion)(int);  // función acción (dummy)
    int prox_estado;         // estado destino
} estado_t;

// ===================== TABLAS =====================
// STOP
estado_t stop[] = {
    {0, nada, S},
    {1, nada, A},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, S},
};

// ACELERAR
estado_t acel[] = {
    {0, nada, S},
    {1, nada, C},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, A},
};

// CONTROL
estado_t control[] = {
    {0, nada, S},
    {1, nada, C},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, C},
};

estado_t* tabla_de_estados[] = { stop, acel, control }; // tabla general

static int estadoActual = S;    // static - recuerda el valor entre llamadas


// FUNCION TRANSICIONAR
static int transicionar(int entrada) {
    estado_t* p = tabla_de_estados[estadoActual];

    // busca el el recibo que coincida con la entrada
    while (p->recibo != entrada && p->recibo != CUALQUIERA) p++;

    //ejecuta las funciones de transicion - dummies
    (p->transicion)(entrada);
    
    // actualizo el estado
    estadoActual = p->prox_estado;

    // EJECUTAR ESTADO
    acciones_estado[estadoActual]();

    return estadoActual;
}

// ESTADO STOP
void estadoStop() {
    digitalWrite(LED_INTERNAL, LOW);
    digitalWrite(LED_EXTERNAL, LOW);
    Serial.println("Estado: STOP");
    // si RUN = 0   se queda en S
    // si RUN = 1   pasa a A
    // si SP=1      se queda en S
    // si SP=0      se queda en C
}

// ESTADO ACEL
void estadoAcel() {
    digitalWrite(LED_INTERNAL, HIGH);
    digitalWrite(LED_EXTERNAL, HIGH);
    Serial.println("Estado: ACEL");
    // si RUN = 0   pasa a S
    // si RUN = 1   se queda en A
    // si SP=1      se queda en A
    // si SP=0      pasa a C
}

// ESTADO CONTROL
void estadoControl() {
    digitalWrite(LED_INTERNAL, LOW);
    digitalWrite(LED_EXTERNAL, HIGH);
    Serial.println("Estado: CONTROL");
    // si RUN = 0 pasa a S
    // si RUN = 1 se queda en c
    // si SP=1, pasa a A
    // si SP=0, se queda en C
}

// ===================== SETUP =====================
void setup() {
    Serial.begin(115200);

    pinMode(LED_INTERNAL, OUTPUT);
    pinMode(LED_EXTERNAL, OUTPUT);

    pinMode(BTN_ON, INPUT);
    pinMode(BTN_SP, INPUT);

    attachInterrupt(digitalPinToInterrupt(BTN_ON),  handleRun,   RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_SP),  handleControl, RISING);

    acciones_estado[estadoActual](); // estado inicial
}

// ===================== LOOP =====================
void loop() {
    // Entrada de 2 bits (SP RUN - 00, 01, 10, 11) → 0, 1, 2, 3
    uint8_t c = (SP << 1) | RUN;
    static uint8_t c_prev = 255;

    transicionar(c);

    delay(10);
}
