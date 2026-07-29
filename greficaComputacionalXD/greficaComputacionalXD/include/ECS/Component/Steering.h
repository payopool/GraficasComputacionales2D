#pragma once
#include "Prerequisitos.h"

namespace ECS {

    /**
     * SteeringType
     * Tipos de comportamiento de movimiento autónomo.
     */
    enum class SteeringType {
        NONE,
        SEEK,
        FLEE,
        ARRIVE,
        WANDER,
        PURSUIT,
        OBSTACLE_AVOIDANCE,
        WAYPOINT  
    };

    /**
     * STRUCT Steering
     * Componente que define el comportamiento de movimiento autónomo.
     */
    struct Steering {
        Steering() = default;

        SteeringType behavior{ SteeringType::NONE };   ///< tipo de comportamiento
        sf::Vector2f target{ 0.f, 0.f };               ///< punto objetivo en espacio 2D
        float speed{ 100.f };                          ///< velocidad base de movimiento
        float arriveRadius{ 50.f };                    ///< radio para desacelerar en arrive
        sf::Vector2f targetVelocity{ 0.f, 0.f };       ///< velocidad del objetivo (para pursuit)

        // Nuevo campo para WAYPOINT
        int currentPoint{ 0 };                         ///< índice del punto actual en el circuito
    };

}
