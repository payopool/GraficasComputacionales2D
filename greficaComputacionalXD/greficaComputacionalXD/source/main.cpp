#include "Prerequisitos.h"
#include "core/Window.h"
#include "core/CShape.h"
#include "ECS/component/Transform.h"
#include "ECS/component/Render.h"
#include "ECS/Systems/Rendersystem.h"


/// Puntero global para la ventana principal.
std::unique_ptr<Window> g_window = nullptr;

/// Figura global de tipo rectángulo.
CShape shape(ShapeType::RECTANGLE);

int main()
{
    /// Crear ventana principal
    g_window = std::make_unique<Window>(800, 600, "window");

    /// Inicializar ImGui
    if (!ImGui::SFML::Init(*g_window->m_window))
    {
        return -1;
    }

    /// Reloj para ImGui
    sf::Clock deltaClock;

    /// Configurar color de la figura
    if (shape.getShape() != nullptr)
    {
        shape.getShape()->setFillColor(sf::Color::Magenta);
    }

    /// Bucle principal
    while (g_window->isOpen())
    {
        /// Procesar eventos
        while (const std::optional event = g_window->m_window->pollEvent())
        {
            ImGui::SFML::ProcessEvent(*g_window->m_window, *event);

            if (event->is<sf::Event::Closed>())
            {
                g_window->close();
            }
        }

        /// Actualizar ImGui
        ImGui::SFML::Update(*g_window->m_window, deltaClock.restart());

        /// Ventana de prueba ImGui
        ImGui::Begin("Prueba ImGui");

        ImGui::Text("Hola Mundo");

        ImGui::Separator();

        ImGui::Text("SFML + ImGui funcionando correctamente");

        ImGui::End();

        /// Limpiar pantalla
        g_window->clear(sf::Color::Black);

        /// Dibujar figura
        shape.draw(*g_window);

        /// Dibujar interfaz ImGui
        ImGui::SFML::Render(*g_window->m_window);

        /// Mostrar contenido
        g_window->display();
    }

    /// Cerrar ImGui
    ImGui::SFML::Shutdown();

    return 0;
}