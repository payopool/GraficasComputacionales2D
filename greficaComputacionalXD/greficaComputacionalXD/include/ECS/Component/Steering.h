#pragma once
#include"Prerequisitos.h"
namespace ECS {
    /**
     *SteeringType
     *Tipos de comportamiento de movimiento autónomo.
     *
     * NONE Sin comportamiento.
     * SEEK La entidad se mueve hacia un objetivo.
     * FLEE  La entidad se aleja del objetivo.
     * ARRIVE La entidad se acerca al objetivo y desacelera al llegar.
     * WANDER Movimiento aleatorio dentro de un área
     * PURSUIT  Perseguir a otra entidad en movimiento
     * OBSTACLE_AVOIDANCE Evitar colisiones con obstáculos
     */
    enum class SteeringType {
        NONE,
        SEEK,
        FLEE,
        ARRIVE,
        WANDER,
        PURSUIT,
        OBSTACLE_AVOIDANCE
    };

        /**
       * STRUCT Steering
       * Componente que define el comportamiento de movimiento autónomo.
       *
       * Este componente permite asignar un comportamiento de Steering a una entidad,
       * incluyendo parámetros como objetivo, velocidad y radio de llegada.
       */
    struct Steering {

        Steering() = default;

        SteeringType behavior{ SteeringType::NONE };//tipo de comportamiento
        sf::Vector2f target{ 0.f,0.f };// putno objetivo espacio 2d
        float speed{ 100.f };//velocidad base de movimiento
        float arriveRadius{ 50.f };//radio para desacelerar en arrive
        sf::Vector2f targetVelocity; ///< Velocidad del objetivo para cálculos de Pursuit

        
    };
  
}