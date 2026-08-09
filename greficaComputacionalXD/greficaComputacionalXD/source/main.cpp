/**
 *  main.cpp
 *  Punto de entrada principal del simulador de carreras con ECS.
 *
 * Este archivo inicializa la ventana, registra los sistemas ECS,
 * crea las entidades (carros, meta, cámara) y ejecuta el bucle principal.
 */

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

 /// Ventana global del simulador
Window g_window(Window(1280, 720, "._."));

/// Registro ECS global
ECS::Registry registry;

/**
 *  Función de limpieza al cerrar la aplicación.
 *
 * Apaga la integración de ImGui con SFML.
 */
void destroy() {
    ImGui::SFML::Shutdown();
}

/**
 *  Función principal del programa.
 *
 * - Registra los sistemas ECS (render, UI, cámara, steering).
 * - Inicializa ImGui.
 * - Crea las entidades: meta, círculo de referencia y tres carros.
 * - Ejecuta el bucle principal de renderizado y actualización.
 *
 * @return int Código de salida (0 si todo fue correcto).
 */
int main() {
    // Registrar sistemas ECS
    registry.AddSystem<ECS::RenderSystem>(g_window);
    registry.AddSystem<ECS::Uisystem>(g_window);
    registry.AddSystem<ECS::CameraSystem>(g_window);
    registry.AddSystem<ECS::SteeringSystem>();

    // Inicializar ImGui con SFML
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

    // Crear entidad meta
    ECS::EntityID metaEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Meta>(metaEntity, ECS::Meta{ metaIndex });

    // Calcular dirección y normal de la meta
    sf::Vector2f metaPos = circuitPoints[metaIndex];
    sf::Vector2f nextPos = circuitPoints[metaIndex + 1];
    sf::Vector2f metaDir = nextPos - metaPos;
    float metaLen = std::sqrt(metaDir.x * metaDir.x + metaDir.y * metaDir.y);
    if (metaLen != 0) metaDir /= metaLen;
    sf::Vector2f metaNormal(-metaDir.y, metaDir.x);

    // Crear entidad círculo de referencia
    ECS::EntityID circle = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Render>(
        circle,
        ECS::Render::Make(CIRCLE, sf::Color::White, "Texture/Cyberpunk.png")
    );

    // Posiciones iniciales en carriles
    sf::Vector2f startYellow = metaPos + metaNormal * 20.f;
    sf::Vector2f startBlue = metaPos - metaNormal * 20.f;
    sf::Vector2f startRed = metaPos;

    // Carro amarillo
    ECS::EntityID carYellow = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(carYellow, startYellow, 0.f);
    registry.AddComponent<ECS::Render>(
        carYellow,
        ECS::Render::Make(CAR, sf::Color::Yellow, "Texture/Carrito.png")
    );
    registry.AddComponent<ECS::Steering>(carYellow, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startYellow,
        40.f + static_cast<float>(std::rand() % 60),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>(metaIndex + 1)
        });

    // Carro azul
    ECS::EntityID carBlue = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(carBlue, startBlue, 0.f);
    registry.AddComponent<ECS::Render>(
        carBlue,
        ECS::Render::Make(CAR, sf::Color::Blue, "Texture/Carrito.png")
    );
    registry.AddComponent<ECS::Steering>(carBlue, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startBlue,
        40.f + static_cast<float>(std::rand() % 60),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>(metaIndex + 2)
        });

    // Carro rojo
    ECS::EntityID carRed = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(carRed, startRed, 0.f);
    registry.AddComponent<ECS::Render>(
        carRed,
        ECS::Render::Make(CAR, sf::Color::Red, "Texture/Carrito.png")
    );
    registry.AddComponent<ECS::Steering>(carRed, ECS::Steering{
        ECS::SteeringType::WAYPOINT,
        startRed,
        40.f + static_cast<float>(std::rand() % 70),
        40.f,
        sf::Vector2f{0.f, 0.f},
        static_cast<int>((metaIndex + 3) % circuitPoints.size())
        });

    // Cámara principal
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
