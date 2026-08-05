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
     * Este sistema aplica las reglas de SEEK, FLEE, ARRIVE, WANDER, etc.
     */
    class SteeringSystem final : public System {
    public:
        /// Constructor con temporizador de inicio
        SteeringSystem() : startDelay(3.f), elapsed(0.f) {}

        /// Actualiza las posiciones de las entidades según su comportamiento Steering.
        void OnUpdate(Registry& registry, float dt) override;

    private:
        float startDelay; // tiempo de espera en segundos
        float elapsed;    // tiempo acumulado
    };

}
