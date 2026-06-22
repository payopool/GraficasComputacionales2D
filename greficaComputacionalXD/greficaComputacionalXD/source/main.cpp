#include "Prerequisitos.h"
#include "Core/Window.h"
#include "Core/CShape.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Systems/RenderSystem.h"

Window g_window(Window(800, 600, "Labrid Engine"));
ECS::Registry registry;

void destroy()
{
    ImGui::SFML::Shutdown();
}

int main()
{
    registry.AddSystem<ECS::RenderSystem>(g_window);

    // m_window es un puntero a sf::RenderWindow.
    if (!ImGui::SFML::Init(*g_window.m_window))
    {
        return -1;
    }

    // Habilitar docking.
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
    bool showDemoWindow = true;

    ECS::EntityID circle = registry.CreateEntity();

    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });

    registry.AddComponent<ECS::Render>(circle, ECS::Render::Make(CIRCLE, sf::Color::Magenta));

    ECS::EntityID tri = registry.CreateEntity();

    registry.AddComponent<ECS::Transform>(tri, sf::Vector2f{ 200.f, 200.f }, 45.f);

    registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));

    ECS::EntityID tri1 = registry.CreateEntity();

    registry.AddComponent<ECS::Transform>(tri1, sf::Vector2f{ 200.f, 200.f }, 45.f);

    registry.AddComponent<ECS::Render>(tri1, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));

    ECS::EntityID tri2 = registry.CreateEntity();

    registry.AddComponent<ECS::Transform>(tri2, sf::Vector2f{ 200.f, 200.f }, 45.f);

    registry.AddComponent<ECS::Render>(tri2, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));



    while (g_window.isOpen()) {
        while (const std::optional event =
            g_window.m_window->pollEvent()) {
            // ImGui debe recibir todos los eventos de SFML.
            ImGui::SFML::ProcessEvent(
                *g_window.m_window,
                *event
            );

            if (event->is<sf::Event::Closed>()) {
                g_window.close();
            }
        }

        const sf::Time elapsedTime = deltaClock.restart();
        const float dt = elapsedTime.asSeconds();

        // Iniciar el frame de ImGui.
        ImGui::SFML::Update(*g_window.m_window, elapsedTime);

        ImGuiDockNodeFlags dockspaceFlags =
            ImGuiDockNodeFlags_PassthruCentralNode;

        ImGui::DockSpaceOverViewport(
            0,
            ImGui::GetMainViewport(),
            dockspaceFlags
        );

        ImGui::ShowDemoWindow(&showDemoWindow);

        ImGui::Begin("Entity Controls");

        if (auto* t = registry.TryGetComponent<ECS::Transform>(circle)) {
            ImGui::Text("Circle");
            ImGui::SliderFloat2("Position##circle", &t->position.x, 0.0f, 800.0f);
        }

        if (auto* t = registry.TryGetComponent<ECS::Transform>(tri)) {
            ImGui::Text("Triangle");
            ImGui::SliderFloat2("Position##tri", &t->position.x, 0.0f, 800.0f);
            ImGui::SliderFloat("Rotation##tri", &t->rotation, 0.0f, 360.0f);
        }
        if (auto* t = registry.TryGetComponent<ECS::Transform>(tri1)) {
            ImGui::Text("Tri1");
            ImGui::SliderFloat2("Position##tri1", &t->position.x, 0.0f, 800.0f);
            ImGui::SliderFloat("Rotation##tri1", &t->rotation, 0.0f, 360.0f);
        }
        if (auto* t = registry.TryGetComponent<ECS::Transform>(tri2)) {
            ImGui::Text("Tri2");
            ImGui::SliderFloat2("Position##tri2", &t->position.x, 0.0f, 800.0f);
            ImGui::SliderFloat("Rotation##tri2", &t->rotation, 0.0f, 360.0f);
        }

        ImGui::End();



        // Limpiar la ventana.
        g_window.clear(sf::Color::Black);

        // Renderizar los elementos de tu ECS.
        registry.UpdateSystems(dt);

        // Renderizar ImGui después de la escena.
        ImGui::SFML::Render(*g_window.m_window);

        // Presentar el frame.
        g_window.display();
       

    }

    destroy();

    return 0;
}