#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"
namespace ECS {
    /**
  * SteeringSystem
  * Sistema encargado de actualizar entidades con comportamiento Steering.
  *
  * Este sistema aplica las reglas de SEEK, FLEE y ARRIVE sobre las entidades
  * que poseen los componentes Transform y Steering.
  */
    class SteeringSystem final : public System {
    public:

        SteeringSystem() = default;

        /**
        * @brief Actualiza las posiciones de las entidades según su comportamiento Steering.
        * @param registry Referencia al registro ECS.
        * @param dt Delta time (tiempo transcurrido entre frames).
        */
        void OnUpdate (Registry& registry, float dt)override;
    };
}