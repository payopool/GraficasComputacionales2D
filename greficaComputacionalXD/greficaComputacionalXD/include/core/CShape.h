#pragma once
#include "Prerequisitos.h"

class Window;

/**
 * @brief Clase que representa una figura geométrica dibujable en una ventana.
 *
 * Esta clase encapsula un objeto de tipo `sf::Shape` de SFML y permite
 * crear diferentes tipos de figuras (círculo, rectángulo, etc.) según
 * el valor de `ShapeType`.
 */
class CShape {
public:
    /**
     * @brief Constructor por defecto.
     */
    CShape() = default;

    /**
     * @brief Constructor que inicializa la figura con un tipo específico.
     * @param shapeType Tipo de figura a crear (Circle, Rectangle, etc.).
     */
    explicit CShape(ShapeType shapeType);

    /**
     * @brief Destructor por defecto.
     */
    ~CShape() = default;

    /**
     * @brief Dibuja la figura en la ventana proporcionada.
     * @param window Referencia a la ventana donde se dibujará la figura.
     */
    void draw(Window& window);

    /**
     * @brief Obtiene el puntero a la figura SFML interna.
     * @return Puntero a `sf::Shape`.
     */
    sf::Shape* getShape();

private:
    /**
     * @brief Crea una figura según el tipo especificado.
     * @param shapeType Tipo de figura a crear.
     * @return Puntero único a la figura creada.
     */
    std::unique_ptr<sf::Shape> createShape(ShapeType shapeType);

    /// Puntero único a la figura SFML.
    std::unique_ptr<sf::Shape> m_shape = nullptr;

    /// Tipo de figura actual.
    ShapeType m_ShapeType;
};
