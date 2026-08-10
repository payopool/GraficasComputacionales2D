#include "ECS/Systems/LeaderboardSystem.h"
#include "Circuit.h"
#include "ECS/Component/Meta.h"

namespace ECS {

    void LeaderboardSystem::OnUpdate(Registry& registry, float dt) {
        ImGui::Begin("Leaderboard en Vivo");

        struct Entry {
            std::string name;
            int lap;
            float currentTime;
            float bestTime;
            float speed;
            int currentPoint;
            float distToNext;
        };

        std::vector<Entry> entries;

        auto circuitPoints = GetCircuitPoints({ 1280, 720 });

        // Obtener índice de meta desde el componente Meta
        size_t metaIndex = 0;
        registry.GetView<Meta>().Each([&](EntityID, Meta& m) {
            metaIndex = m.index;
            });

        // Recorrer todos los carros
        registry.GetView<Transform, Steering, LapData>().Each(
            [&](EntityID id, Transform& t, Steering& s, LapData& lap) {
                lap.currentTime += dt;
                lap.speed = s.speed;

                // Calcular distancia al siguiente waypoint
                sf::Vector2f target = circuitPoints[s.currentPoint];
                float distToNext = std::hypot(target.x - t.position.x,
                    target.y - t.position.y);

                // Detectar cruce de meta -> actualizar vueltas y récord
                if (s.currentPoint == metaIndex) {
                    if (lap.currentTime > 1.0f) { // evita múltiples registros seguidos
                        lap.lap++;
                        if (lap.bestTime == 0.f || lap.currentTime < lap.bestTime)
                            lap.bestTime = lap.currentTime;
                        lap.currentTime = 0.f;
                    }
                }

                entries.push_back({
                    "Carro " + std::to_string(id),
                    lap.lap,
                    lap.currentTime,
                    lap.bestTime,
                    lap.speed,
                    s.currentPoint,
                    distToNext
                    });
            }
        );

        // Ordenar por progreso en pista
        std::sort(entries.begin(), entries.end(),
            [](const Entry& a, const Entry& b) {
                if (a.lap != b.lap)
                    return a.lap > b.lap; // más vueltas completadas primero
                if (a.currentPoint != b.currentPoint)
                    return a.currentPoint > b.currentPoint; // más adelante en el circuito
                return a.distToNext < b.distToNext;        // más cerca del siguiente punto
            });

        // Dibujar tabla
        if (ImGui::BeginTable("Leaderboard", 6)) {
            ImGui::TableSetupColumn("Pos");
            ImGui::TableSetupColumn("Piloto");
            ImGui::TableSetupColumn("Vta");
            ImGui::TableSetupColumn("Actual");
            ImGui::TableSetupColumn("Record");
            ImGui::TableSetupColumn("Velocidad");
            ImGui::TableHeadersRow();

            int pos = 1;
            for (auto& e : entries) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%d", pos++);
                ImGui::TableSetColumnIndex(1); ImGui::Text("%s", e.name.c_str());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%d", e.lap);
                ImGui::TableSetColumnIndex(3); ImGui::Text("%.2f", e.currentTime);
                ImGui::TableSetColumnIndex(4); ImGui::Text("%.2f", e.bestTime);
                ImGui::TableSetColumnIndex(5); ImGui::Text("%.1f px/s", e.speed);
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }

}
