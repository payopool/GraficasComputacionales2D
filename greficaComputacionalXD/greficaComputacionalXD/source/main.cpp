#include "Prerequisitos.h"
#include "core/Window.h"
#include "core/CShape.h" 



/// Puntero global para la ventana principal.
std::unique_ptr<Window> g_window = nullptr;

/// Figura global de tipo círculo.
CShape shape(ShapeType::RECTANGLE);

/**
 * @brief Función principal del programa.
 *
 * - Crea la ventana principal.
 * - Configura la figura inicial.
 * - Ejecuta el bucle principal del juego:
 *   - Manejo de eventos.
 *   - Limpieza de pantalla.
 *   - Dibujado de figuras.
 *   - Presentación en pantalla.
 *
 * @return Código de salida del programa (0 = éxito).
 */
int main()
{
    /// Crear ventana de 800x600 con título "window".
    g_window = std::make_unique<Window>(800, 600, "window");

    /// Configurar color de la figura si existe.
    if (shape.getShape() != nullptr) {
        shape.getShape()->setFillColor(sf::Color::Magenta);
    }

    /// Bucle principal del juego.
    while (g_window->isOpen())
    {
        // Manejo de eventos
        while (const std::optional event = g_window->m_window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                g_window->close();
        }

        // Limpiar la pantalla con color negro
        g_window->clear(sf::Color::Black);

        // Dibujar la figura usando su propio método draw
        shape.draw(*g_window);

        // Mostrar lo que se dibujó en la pantalla
        g_window->display();
    }

    /// Finalización del programa
    return 0;
}
