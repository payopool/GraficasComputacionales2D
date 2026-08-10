#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"
#include "ECS/Component/LapData.h"

namespace ECS {
    /**
     *  LeaderboardSystem
     *  Sistema ECS encargado de mostrar la tabla de posiciones en vivo.
     *
     * Calcula el progreso de cada carro en el circuito y ordena la tabla
     * según el índice de waypoint y la distancia al siguiente punto.
     */
    class LeaderboardSystem final : public System {
    public:
        void OnUpdate(Registry& registry, float dt) override;
    };
}
