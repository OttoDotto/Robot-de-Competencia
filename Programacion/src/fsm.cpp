/**
 @file fsm.cpp
 @brief Implementación de la Máquina de Estados Finitos (FSM).
 @details Controla el flujo de operación del robot entre los estados de parada, aceleración y control PID 
 mediante tablas de transición basadas en las banderas de RUN y SETPOINT.
 @author Legion de Ohm
 */

#include "fsm.hpp"

/**
 @brief Función de acción nula (dummy) utilizada para transiciones que no requieren lógica adicional.
 @param c Código de entrada.
 @return int Siempre retorna 0.
 */
int nada(int c) { return 0; }

/**
 @struct estado_t
 @brief Estructura que define una fila en la tabla de transiciones de un estado.
 */
typedef struct {
    int recibo;               ///< Combinación binaria de señales de entrada (SP, RUN).
    int (*transicion)(int);   ///< Puntero a función de acción de transición (dummy).
    int prox_estado;          ///< Identificador del siguiente estado.
} estado_t;

// ===================== TABLAS =====================

/** @brief Tabla de transiciones para el estado STOP (S). 
 */
estado_t stop[] = {
    {0, nada, S},
    {1, nada, A},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, S},
};

/** @brief Tabla de transiciones para el estado ACELERAR (A). 
 */
estado_t acel[] = {
    {0, nada, S},
    {1, nada, C},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, A},
};

/** @brief Tabla de transiciones para el estado CONTROL (C). 
 */
estado_t control[] = {
    {0, nada, S},
    {1, nada, C},
    {2, nada, S},
    {3, nada, A},
    {CUALQUIERA, nada, C},
};

/** @brief Tabla general que agrupa los punteros a las tablas de cada estado. 
 */
estado_t* tabla_de_estados[] = { stop, acel, control };

/** @brief Punteros a las funciones de acción de cada estado, definidas externamente. 
 */
extern void (*acciones_estado[])();

/** @brief Variable estática que mantiene el rastro del estado actual de la FSM. 
 */
static int estadoActual = S;

// ==================== FUNCION FSM ====================

/**
 @brief Ejecuta la lógica de transición de la máquina de estados.
 @details Busca en la tabla del estado actual una coincidencia con la entrada proporcionada. 
 Si la encuentra, actualiza el estado y ejecuta la acción correspondiente al nuevo estado.
 @param entrada Valor entero que combina las señales de control para decidir la transición.
 @return int El nuevo estado después de procesar la entrada.
 */
int transicionar(int entrada) {
    estado_t* p = tabla_de_estados[estadoActual];

    // busca el recibo que coincida con la entrada
    while (p->recibo != entrada && p->recibo != CUALQUIERA) p++;

    // ejecuta las funciones de transicion - dummies (como no las ejecutamos lo dejaremos comentado)
    //(p->transicion)(entrada);
    
    // actualizo el estado
    estadoActual = p->prox_estado;

    // EJECUTAR ESTADO
    acciones_estado[estadoActual]();

    return estadoActual;
}
