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
#include <imgui.h>
void SetCyberpunkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Fondo oscuro
    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.08f, 1.0f);

    // Headers y tabs en azul eléctrico
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.40f, 0.80f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.55f, 0.95f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.65f, 1.00f, 1.0f);

    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.40f, 0.80f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.55f, 0.95f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.65f, 1.00f, 1.0f);

    // Botones en rosa neón
    colors[ImGuiCol_Button] = ImVec4(0.80f, 0.00f, 0.50f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.20f, 0.70f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.40f, 0.80f, 1.0f);

    // Sliders y campos con acento morado
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.10f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.15f, 0.40f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.20f, 0.55f, 1.0f);

    // Ajustes de estilo
    style.FrameRounding = 8.0f; // bordes redondeados
    style.GrabRounding = 8.0f;
    style.WindowBorderSize = 1.0f;
}


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
    registry.AddSystem<ECS::CameraSystem>(g_window); // sistema de cámara

    if (!ImGui::SFML::Init(*g_window.m_window))
        return -1;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    
    SetCyberpunkTheme();

    sf::Clock deltaClock;
    bool showDemoWindow = true;

    // Crear entidades de figuras
    ECS::EntityID circle = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Render>(
        circle,
        ECS::Render::Make(CIRCLE, sf::Color::White, "Texture/Cyberpunk.png")
    );


    ECS::EntityID tri = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri, sf::Vector2f{ 200.f, 200.f }, 45.f);
    registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));


    // Crear entidad cámara
    ECS::EntityID cameraEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(cameraEntity, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Camera>(cameraEntity, ECS::Camera{
        true,              // activa
        tri,            // sigue al círculo
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


