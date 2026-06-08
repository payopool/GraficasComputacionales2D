#include "Prerequisitos.h"
#include "core/Window.h"
#include "core/CShape.h" // Cuidado con las mayúsculas en el nombre del archivo

std::unique_ptr<Window> g_window = nullptr; // Puntero global para la ventana
CShape shape(ShapeType::CIRCLE); // Creamos tu figura tipo círculo



int main()
{
    
	g_window = std::make_unique<Window>(800, 600, "window");

   
    if (shape.getShape() != nullptr) {
        shape.getShape()->setFillColor(sf::Color::Magenta);
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
 
    return 0;
}