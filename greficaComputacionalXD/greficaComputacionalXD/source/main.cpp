#include "Prerequisitos.h"
#include "Core/Window.h"
#include "Core/CShape.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Camera.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/Uisystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Component/Steering.h"
#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Component/Meta.h"  

Window g_window(Window(1280, 720, "._."));
ECS::Registry registry;

void destroy() {
    ImGui::SFML::Shutdown();
}

int main() {
    // Registrar sistemas ECS
    registry.AddSystem<ECS::RenderSystem>(g_window);
    registry.AddSystem<ECS::Uisystem>(g_window);
    registry.AddSystem<ECS::CameraSystem>(g_window);
    registry.AddSystem<ECS::SteeringSystem>();

    if (!ImGui::SFML::Init(*g_window.m_window))
        return -1;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
    bool showDemoWindow = true;

    // Obtener puntos del circuito
    auto circuitPoints = GetCircuitPoints({ 1280, 720 });
    size_t metaIndex = 250;
    if (metaIndex >= circuitPoints.size()) metaIndex = circuitPoints.size() / 2;

    // Crear entidad meta (ahora sí ya existe metaIndex)
    ECS::EntityID metaEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Meta>(metaEntity, ECS::Meta{ metaIndex });

    sf::Vector2f metaPos = circuitPoints[metaIndex];
    sf::Vector2f nextPos = circuitPoints[metaIndex + 1];

    sf::Vector2f metaDir = nextPos - metaPos;
    float metaLen = std::sqrt(metaDir.x * metaDir.x + metaDir.y * metaDir.y);
    if (metaLen != 0) metaDir /= metaLen;

    sf::Vector2f metaNormal(-metaDir.y, metaDir.x);

    // Crear entidad círculo
    ECS::EntityID circle = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Render>(
        circle,
        ECS::Render::Make(CIRCLE, sf::Color::White, "Texture/Cyberpunk.png")
    );

    // posiciones iniciales en carriles
    sf::Vector2f startYellow = metaPos + metaNormal * 20.f;
    sf::Vector2f startBlue = metaPos - metaNormal * 20.f;
    sf::Vector2f startRed = metaPos;

    // Triángulo amarillo
    ECS::EntityID triYellow = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(triYellow, startYellow, 0.f);
    registry.AddComponent<ECS::Render>(triYellow, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));
    registry.AddComponent<ECS::Steering>(triYellow, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startYellow,
        40.f + static_cast<float>(std::rand() % 70),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>(metaIndex + 1)
        });

    // Triángulo azul
    ECS::EntityID triBlue = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(triBlue, startBlue, 0.f);
    registry.AddComponent<ECS::Render>(triBlue, ECS::Render::Make(TRINAGLE, sf::Color::Blue));
    registry.AddComponent<ECS::Steering>(triBlue, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startBlue,
        40.f + static_cast<float>(std::rand() % 70),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>(metaIndex + 2)
        });

    // Triángulo rojo
    ECS::EntityID triRed = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(triRed, startRed, 0.f);
    registry.AddComponent<ECS::Render>(triRed, ECS::Render::Make(TRINAGLE, sf::Color::Red));
    registry.AddComponent<ECS::Steering>(triRed, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startRed,
        40.f + static_cast<float>(std::rand() % 70),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>(metaIndex + 3)
        });

    // Cámara
    ECS::EntityID cameraEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(cameraEntity, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Camera>(cameraEntity, ECS::Camera{
        true,
        circle,
        2.0f,
        1.0f
        });

    // Bucle principal
    while (g_window.isOpen()) {
        while (const std::optional event = g_window.m_window->pollEvent()) {
            ImGui::SFML::ProcessEvent(*g_window.m_window, *event);

            if (event->is<sf::Event::Closed>()) {
                g_window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                g_window.handleResize(resized->size);
            }
        }

        const sf::Time elapsedTime = deltaClock.restart();
        const float dt = elapsedTime.asSeconds();

        ImGui::SFML::Update(*g_window.m_window, elapsedTime);

        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspaceFlags);

        ImGui::ShowDemoWindow(&showDemoWindow);

        g_window.clear(sf::Color::Black);

        registry.UpdateSystems(dt);

        ImGui::SFML::Render(*g_window.m_window);
        g_window.display();
    }

    destroy();
    return 0;
}
