/**
 *  main.cpp
 *  Punto de entrada principal de la aplicación de gráficas computacionales 2D con ECS.
 *
 * Este archivo inicializa la ventana, configura ImGui con un tema personalizado,
 * registra los sistemas ECS (Render, UI, Cámara, Steering) y crea las entidades
 * que forman parte de la escena (círculo, triángulos y cámara).
 *
 * La aplicación corre en un bucle principal que procesa eventos, actualiza sistemas
 * y renderiza la interfaz gráfica junto con las figuras en pantalla.
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
#include <imgui.h>

 /// Configura un tema visual estilo "Cyberpunk" para ImGui.
void SetCyberpunkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.08f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.40f, 0.80f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.55f, 0.95f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.65f, 1.00f, 1.0f);

    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.40f, 0.80f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.55f, 0.95f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.65f, 1.00f, 1.0f);

    colors[ImGuiCol_Button] = ImVec4(0.80f, 0.00f, 0.50f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.20f, 0.70f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.40f, 0.80f, 1.0f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.10f, 0.25f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.35f, 0.15f, 0.40f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.20f, 0.55f, 1.0f);

    style.FrameRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.WindowBorderSize = 1.0f;
}

/// Ventana principal de la aplicación.
Window g_window(Window(800, 600, "._."));
/// Registro ECS que administra entidades, componentes y sistemas.
ECS::Registry registry;

/// Libera recursos de ImGui al cerrar la aplicación.
void destroy() {
    ImGui::SFML::Shutdown();
}

/**
 *  Función principal de la aplicación.
 *
 * - Inicializa ImGui y los sistemas ECS.
 * - Crea las entidades de la escena (círculo y triángulos).
 * - Configura la cámara.
 * - Ejecuta el bucle principal de eventos, actualización y renderizado.
 *
 * Código de salida (0 si la ejecución fue exitosa).
 */

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

    SetCyberpunkTheme();

    sf::Clock deltaClock;
    bool showDemoWindow = true;

    // Crear entidad círculo
    ECS::EntityID circle = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Render>(
        circle,
        ECS::Render::Make(CIRCLE, sf::Color::White, "Texture/Cyberpunk.png")
    );
 
    // Crear triángulo amarillo
    ECS::EntityID tri = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri, sf::Vector2f{ 260.f, 230.f }, 45.f);
    registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRINAGLE, sf::Color::Yellow));
    registry.AddComponent<ECS::Steering>(tri, ECS::Steering{
        ECS::SteeringType::NONE,
        sf::Vector2f{400.f, 300.f},
        120.f,
        50.f
        });

    // Crear triángulo azul
    ECS::EntityID tri1 = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri1, sf::Vector2f{ 450.f, 430.f }, 45.f);
    registry.AddComponent<ECS::Render>(tri1, ECS::Render::Make(TRINAGLE, sf::Color::Blue));
    registry.AddComponent<ECS::Steering>(tri1, ECS::Steering{
        ECS::SteeringType::NONE,
        sf::Vector2f{400.f, 300.f},
        120.f,
        50.f
        });

    // Crear triángulo rojo
    ECS::EntityID tri2 = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(tri2, sf::Vector2f{ 350.f, 350.f }, 45.f);
    registry.AddComponent<ECS::Render>(tri2, ECS::Render::Make(TRINAGLE, sf::Color::Red));
    registry.AddComponent<ECS::Steering>(tri2, ECS::Steering{
        ECS::SteeringType::NONE,
        sf::Vector2f{400.f, 300.f},
        120.f,
        50.f
        });

    // Crear entidad cámara
    ECS::EntityID cameraEntity = registry.CreateEntity();
    registry.AddComponent<ECS::Transform>(cameraEntity, sf::Vector2f{ 400.f, 300.f });
    registry.AddComponent<ECS::Camera>(cameraEntity, ECS::Camera{
        true,   // activa
        circle, // sigue al círculo
        2.0f,   // velocidad de seguimiento
        1.0f    // zoom inicial
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

        // Actualizar sistemas (Render, UI, Camera, Steering)
        registry.UpdateSystems(dt);

        ImGui::SFML::Render(*g_window.m_window);
        g_window.display();
    }

    destroy();
    return 0;
}
