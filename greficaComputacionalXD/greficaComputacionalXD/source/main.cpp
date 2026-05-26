#include "Prerequisitos.h"
#include "core/Window.h"
#include "core/CShape.h" // Cuidado con las mayúsculas en el nombre del archivo

Window* g_window = nullptr;
CShape shape(ShapeType::CIRCLE); // Creamos tu figura tipo círculo

void destroy() {
    SAFE_PTR_RELEASE(g_window);
}

int main()
{
    // 1. Crear la ventana
    g_window = new Window(800, 600, "My window");

    // 2. Cambiar el color de la figura correctamente
    // Obtenemos el puntero de SFML con getShape() y le asignamos el color
    if (shape.getShape() != nullptr) {
        shape.getShape()->setFillColor(sf::Color(100, 250, 50));
    }

    // Bucle principal del juego
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

        // 3. Dibujar la figura usando su propio método draw
        // Le pasamos la referencia de nuestra ventana (*g_window)
        shape.draw(*g_window);

        // Mostrar lo que se dibujó en la pantalla
        g_window->display();
    }

    // Limpieza de memoria al cerrar
    destroy();
    return 0;
}