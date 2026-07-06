#pragma once
#pragma once
#include "Prerequisitos.h"
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
        /// Constructor por defecto.
        SteeringSystem() = default;

        /**
         * Actualiza las posiciones de las entidades según su comportamiento Steering.
         * registry Referencia al registro ECS.
         * dt Delta time (tiempo transcurrido entre frames).
         */
        void OnUpdate(Registry& registry, float dt) override;
    };

}
