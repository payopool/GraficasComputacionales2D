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

    // Crear entidades
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

    while (g_window.isOpen()) {
        while (const std::optional event = g_window.m_window->pollEvent()) {
            ImGui::SFML::ProcessEvent(*g_window.m_window, *event);

            if (event->is<sf::Event::Closed>()) {
                g_window.close();
            }
        }

        const sf::Time elapsedTime = deltaClock.restart();
        const float dt = elapsedTime.asSeconds();

        // Iniciar el frame de ImGui.
        ImGui::SFML::Update(*g_window.m_window, elapsedTime);

        ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspaceFlags);

        ImGui::ShowDemoWindow(&showDemoWindow);

        // Inspector de entidades
        ImGui::Begin("Entity Controls");

        auto drawInspector = [&](const char* name, ECS::EntityID id) {
            if (auto* t = registry.TryGetComponent<ECS::Transform>(id)) {
                auto* r = registry.TryGetComponent<ECS::Render>(id);
                ImGui::Separator();
                ImGui::Text("%s", name);
                ImGui::SliderFloat2(std::string("Position##").append(name).c_str(), &t->position.x, 0.0f, 800.0f);
                ImGui::SliderFloat(std::string("Rotation##").append(name).c_str(), &t->rotation, 0.0f, 360.0f);

                if (r) {
                    float color[3] = {
                        r->fillColor.r / 255.0f,
                        r->fillColor.g / 255.0f,
                        r->fillColor.b / 255.0f
                    };
                    if (ImGui::ColorEdit3(std::string("Color##").append(name).c_str(), color)) {
                        r->fillColor = sf::Color(
                            static_cast<uint8_t>(color[0] * 255),
                            static_cast<uint8_t>(color[1] * 255),
                            static_cast<uint8_t>(color[2] * 255)
                        );
                    }
                }
            }
            };

        drawInspector("Circle", circle);
        drawInspector("Triangle", tri);
        drawInspector("Tri1", tri1);
        drawInspector("Tri2", tri2);

        ImGui::End();

        // Limpiar la ventana.
        g_window.clear(sf::Color::Black);

        // Renderizar ECS.
        registry.UpdateSystems(dt);

        // Renderizar ImGui después de la escena.
        ImGui::SFML::Render(*g_window.m_window);

        // Presentar el frame.
        g_window.display();
    }

    destroy();
    return 0;
}
