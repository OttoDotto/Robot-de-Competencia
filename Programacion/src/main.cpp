/**
 @file main.cpp
 @brief Archivo principal del Seguidor de Línea Competencia Robótica.
 @details Contiene el flujo principal del programa, la inicialización del hardware, 
 el lazo de control (loop) y las definiciones de las funciones de estado de la FSM.
 @author Legion de Ohm
 */

#include <Arduino.h>
//#include <IRremote.hpp>
#include "buzzer.hpp"
#include "interrupciones.hpp"
#include "config.hpp"
#include "pid.hpp"
#include "sensores.hpp"
#include "motores.hpp"
#include "fsm.hpp"

// VELOCIDADES  - PORCENTAJE DE PWM (0-100%)
/** @brief Límite máximo de velocidad de los motores (PWM %). */
const int32_t maxSpeed  = 90;  

/** @brief Velocidad variable para la rampa de aceleración inicial. */
int32_t velocidadAcel = 50;     

// SETPOINT y ZONA MUERTA
/** @brief Valor objetivo de lectura para estar centrado sobre la línea. */
uint16_t setpoint = 3500;       

/** @brief Margen de error aceptable alrededor del setpoint. */
uint16_t zonaMuerta = 50;      

/* // CONTROL IR - comentado por ahora
// ============================
// COMANDOS CONTROL
// ============================
#define CMD_ON_OFF   0x43           //Tecla 'play/pause'
#define RAW_ON_OFF   0xBC43FF00

void leer_IR() {
// Si recibe algo lo leemos/decodificamos
    if (IrReceiver.decode()) {
        // Guardamos los datos y separamos el dato crudo y el codigo
        auto data = IrReceiver.decodedIRData;
        uint8_t cmd  = data.command;

        // Si coinciden el codigo crudo y comando entonces arranco o paro el coche
        if (cmd == CMD_ON_OFF)   { RUN = !RUN;}

        // Liberamos
        IrReceiver.resume();
    }
}*/

/** @brief Array de punteros a funciones que vincula los estados con sus acciones. */
void (*acciones_estado[])() = { estadoStop, estadoAcel, estadoControl };

/** @brief Bandera para asegurar que la lógica de parada se ejecute una sola vez. */
bool stop_done = false;         


// ============================
// SETUP
// ============================
/**
 @brief Configuración inicial del microcontrolador.
 @details Inicializa periféricos (Serial, Buzzer, Motores, Sensores) y configura 
 las interrupciones y modos de pines.
 */
void setup() {
    // Inicializar Serial, Control-IR SOLOS SI se habilitaron en el PLATFORMIO.INI
    deb(Serial.begin(115200);)
    //control_ir( IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);)
    
    // Configuracion buzzer
    setupBuzzer();
    
    // Configuracion motores - pines de direcion, canal de pwm, frecuencia y resolucion
    setupMotores();

    // Configuracion pines
    pinMode(ledMotores, OUTPUT);
    pinMode(ledCalibracion, OUTPUT);
    pinMode(BTN_RUN, INPUT);
    pinMode(BTN_STOP, INPUT);

    // Configuracion interrupciones
    setupInterrupciones(); 

    // Configuracion y calibracion de sensores
    setupSensores();
}


// ============================
// LOOP
// ============================
/**
 @brief Bucle principal del programa.
 @details Calcula la entrada combinada (RUN y SETPOINT) y llama a la FSM para 
 transicionar y ejecutar el estado correspondiente.
 */
void loop() {
    // Entrada de 2 bits (SETPOINT RUN - 00, 01, 10, 11) → 0, 1, 2, 3
    uint8_t c = (SETPOINT << 1) | RUN;

    // Realizamos la transicion y ejecutamos su estado 
    transicionar(c);
}


// ESTADO STOP - FUNCION DETENIDO
/**
 @brief Acción ejecutada en el estado de parada (STOP).
 @details Detiene los motores, apaga los LEDs de estado y reinicia la velocidad de aceleración.
 */
void estadoStop() {
    if (!stop_done) {                  // solo ejecuta una vez
        deb(Serial.println("Estado: STOP");)

        digitalWrite(ledMotores, LOW);
        digitalWrite(ledCalibracion, LOW);

        detenerMotores();
        
        velocidadAcel = 50;

        stop_done = true;
        deb(Serial.println("\n ---------------------- \n");)
    }
}


// ESTADO ACEL - FUNCION ACELERAR EN LINEA
/**
 @brief Acción ejecutada en el estado de aceleración (ACEL).
 @details Realiza un incremento progresivo de la velocidad mientras el robot se encuentre 
 dentro del setpoint para romper la inercia de manera suave.
 */
void estadoAcel() {
    deb(Serial.println("Estado: ACEL");)
    stop_done = false; // para que cuando vuelva a STOP se ejecute 1 vez
    
    // Leer posicion de línea (0 = extremo izquierda, 7000 = extremo derecha)  
    position = leerLinea();
    deb(Serial.printf("Posicion=%d\n", position);)

    // Incremento suave de velocidad
    if (velocidadAcel < maxSpeed) velocidadAcel++;

    // Mover motores con aceleracion progresiva
    moverMotores(velocidadAcel, velocidadAcel);

    // Indicador de que estamos en setpoint
    digitalWrite(ledCalibracion, HIGH);

    // Calculamos si estamos en el setpoint
    actualizarSP(position);

    // Reiniciamos las variables PID
    reiniciar_pid();

    deb(Serial.println("\n ---------------------- \n");)
}


// ESTADO CONTROL - FUNCION CONTROL EN LINEA
/**
 @brief Acción ejecutada en el estado de control activo (CONTROL).
 @details Ejecuta el algoritmo PID a intervalos fijos marcados por el temporizador 
 para corregir la trayectoria del robot sobre la línea.
 */
void estadoControl() {
    // Ejecutar solo cuando el timer indique el tick
    if (!has_expired) return;

    deb(Serial.println("Estado: CONTROL");)
    has_expired = false;    // ya paso un tick (timer isr) entonces debo reiniciarlo
    stop_done = false;      // cuando vuelva a STOP se ejecute 1 vez

    // Enceder led modo corredor
    digitalWrite(ledMotores, HIGH);

    // Apagar led cal - indicamos que no estamos en Setpoint 
    digitalWrite(ledCalibracion, LOW);

    // Leer posicion de línea (0 = extremo izquierda, 7000 = extremo derecha)    
    position = leerLinea();
    deb(Serial.printf("Posicion=%d\n", position);)

    // calculo la correccion para los motores segun la posicion y el delta tiempo (timer isr) 
    float correcion = calculo_pid(position, FIXED_DT_S);
 
    // Calculamos si estamos en el setpoint
    actualizarSP(position);

    // Control de motores
    controlMotores(correcion);

    // Mover los motores (Avanza, retrocede o para)
    moverMotores(motorSpeedIzq, motorSpeedDer);

    deb(Serial.println("\n ---------------------- \n");)
}