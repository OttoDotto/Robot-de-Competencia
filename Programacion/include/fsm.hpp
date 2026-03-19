/**
 @file fsm.hpp
 @brief Definición de la Máquina de Estados Finitos (FSM) para la lógica de navegación del seguidor de línea.
 @author Legion de Ohm
 */

#pragma once

/**
 @enum estados
 @brief Identificadores de los estados lógicos del seguidor de línea.
 */
enum estados { 
    CUALQUIERA = -1,  ///< Estado para transiciones genéricas.
    S,                ///< Estado STOP: Robot detenido y a la espera.
    A,                ///< Estado ACEL: Fase de arranque o aceleración inicial.
    C,                ///< Estado CONTROL: Fase de seguimiento de línea con PID activo.
    CANT_ESTADOS      ///< Auxiliar para conocer el número total de estados definidos.
};

// ==================== Interfaz de la FSM ====================

/**
 @brief Gestiona el cambio de estado basado en una entrada específica.
 @param entrada Valor entero que representa el evento o condición de disparo.
 @return int El nuevo estado resultante después de la transición.
 */
int transicionar(int entrada);

// ==================== Acciones de estado ====================

/**
 @brief Acción ejecutada durante el estado de parada (S).
 @details Detiene los motores y resetea variables de control si es necesario.
 @return void
 */
void estadoStop();

/**
 @brief Acción ejecutada durante la fase de aceleración (A).
 @details Aplica un perfil de velocidad creciente para romper la inercia inicial.
 @return void
 */
void estadoAcel();

/**
 @brief Acción ejecutada durante la fase de control activo (C).
 @details Ejecuta la lectura de sensores, cálculo de PID y ajuste de motores en tiempo real.
 @return void
 */
void estadoControl();
