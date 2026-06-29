#include "Prerequisitos.h"
#include "Core/Window.h"
#include "Core/CShape.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Camera.h"          // 👈 nuevo
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/Uisystem.h"
#include "ECS/Systems/CameraSystem.h"      // 👈 nuevo

Window g_window(Window(800, 600, "._."));
ECS::Registry registry;

void destroy()
{
    ImGui::SFML::Shutdown();
}

int main()
{
    // Registrar sistemas
    registry.AddSystem<ECS::RenderSystem>(g_window);
    registry.AddSystem<ECS::Uisystem>();
    registry.AddSystem<ECS::CameraSystem>(g_window); // 👈 sistema de cámara

    if (!ImGui::SFML::Init(*g_window.m_window))
        return -1;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
    bool showDemoWindow = true;

    // Crear entidades de figuras
    ECS::EntityID circle = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Render>(circle, ECS::Render::Make(CIRCLE, sf::Color::Magenta));

    ECS::EntityID tri = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri, sf::Vector2f{ 200.f, 200.f }, 45.f);
    registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));

    ECS::EntityID tri1 = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri1, sf::Vector2f{ 300.f, 250.f }, 30.f);
    registry.AddComponent<ECS::Render>(tri1, ECS::Render::Make(TRINAGLE, sf::Color::Cyan));

    ECS::EntityID tri2 = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri2, sf::Vector2f{ 500.f, 350.f }, 60.f);
    registry.AddComponent<ECS::Render>(tri2, ECS::Render::Make(TRINAGLE, sf::Color::Green));

    // Crear entidad cámara
    ECS::EntityID cameraEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(cameraEntity, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Camera>(cameraEntity, ECS::Camera{
        true,              // activa
        circle,            // sigue al círculo
        2.0f,              // velocidad de seguimiento
        1.0f               // zoom inicial
        });

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

        // Actualizar sistemas (Render, UI, Camera)
        registry.UpdateSystems(dt);

        ImGui::SFML::Render(*g_window.m_window);
        g_window.display();
    }

    destroy();
    return 0;
}


