#include "core/CShape.h"
#include "core/Window.h"

/**
 * @brief Constructor de CShape.
 *
 * Inicializa la figura según el tipo especificado.
 *
 * @param shapeType Tipo de figura (Circle, Rectangle, Triangle, etc.).
 */
CShape::CShape(ShapeType shapeType) : m_ShapeType(shapeType) {
    m_shape = createShape(shapeType);
}

/**
 * @brief Dibuja la figura en la ventana.
 *
 * @param window Referencia a la ventana donde se dibujará la figura.
 */
void CShape::draw(Window& window) {
    if (m_shape) {
        window.draw(*m_shape);
    }
}

/**
 * @brief Obtiene el puntero a la figura SFML.
 *
 * @return Puntero a la figura (`sf::Shape*`).
 */
sf::Shape* CShape::getShape() {
    return m_shape.get();
}

/**
 * @brief Crea una figura SFML según el tipo especificado.
 *
 * @param shapeType Tipo de figura.
 * @return Puntero único a la figura creada.
 */
std::unique_ptr<sf::Shape> CShape::createShape(ShapeType shapeType) {
    switch (shapeType) {
    case EMPTY:
        return nullptr;

    case CIRCLE: {
        auto circle = std::make_unique<sf::CircleShape>(50.f);
        circle->setFillColor(sf::Color::Magenta);
        circle->setPosition({ 100.f, 100.f });
        return circle;
    }

    case RECTANGLE: {
        auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f(120.f, 60.f));
        rectangle->setFillColor(sf::Color::Cyan);
        rectangle->setPosition({ 300.f, 100.f });
        return rectangle;
    }

    case TRINAGLE: {
        auto triangle = std::make_unique<sf::ConvexShape>(3);
        triangle->setPoint(0, { 0.f, 0.f });
        triangle->setPoint(1, { 100.f, 0.f });
        triangle->setPoint(2, { 50.f, 100.f });
        triangle->setFillColor(sf::Color::Yellow);
        triangle->setPosition({ 500.f, 100.f });
        return triangle;
    }

    case POLYGON: {
        auto polygon = std::make_unique<sf::ConvexShape>(5);
        polygon->setPoint(0, { 0.f, 0.f });
        polygon->setPoint(1, { 100.f, 0.f });
        polygon->setPoint(2, { 120.f, 50.f });
        polygon->setPoint(3, { 60.f, 100.f });
        polygon->setPoint(4, { -20.f, 50.f });
        polygon->setFillColor(sf::Color::Green);
        polygon->setPosition({ 700.f, 100.f });
        return polygon;
    }

    case SQUARE: {
        auto square = std::make_unique<sf::RectangleShape>(sf::Vector2f(80.f, 80.f));
        square->setFillColor(sf::Color::Red);
        square->setPosition({ 900.f, 100.f });
        return square;
    }

    case CAR: {
        // Rectángulo pequeño para simular un coche visto desde arriba
        auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(40.f, 20.f));

        // Centrar el origen para que las rotaciones sean naturales
        rect->setOrigin(sf::Vector2f(20.f, 7.5f));

        // Color por defecto (luego lo cambias con Render)
        rect->setFillColor(sf::Color::White);

        return rect;
    }




    default:
        return nullptr;
    }
}
