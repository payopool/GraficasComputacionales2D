#pragma once

namespace ECS {
    /**
     *  LapData
     *  Componente ECS para almacenar datos de carrera de cada carro.
     *
     * - lap: número de vuelta actual
     * - currentTime: tiempo acumulado en la vuelta
     * - bestTime: mejor tiempo registrado
     * - speed: velocidad actual en px/s
     */
    struct LapData {
        int lap = 1;
        float currentTime = 0.f;
        float bestTime = 0.f;
        float speed = 0.f;
    };
}

