#pragma once
#include "Prerequisitos.h"

namespace ECS {

    /**
     * Componente de renderizado para entidades.
     *
     * Define la figura SFML asociada a la entidad, su color de relleno
     * y si debe ser visible en pantalla.
     */
    struct Render {
        /// Figura SFML asociada a la entidad.
        std::shared_ptr<sf::Shape> shape;
        sf::Color color;
        /// Color de relleno de la figura.
        sf::Color fillColor{ sf::Color::White };

        /// Indica si la entidad es visible en pantalla.
        bool visisble{ true };

        /**
         * Constructor por defecto.
         *
         * Inicializa el componente sin figura y con color blanco.
         */
        Render() = default;

        /**
         * Constructor parametrizado.
         *
         * s Puntero compartido a la figura SFML.
         * color Color de relleno (por defecto blanco).
         */
        explicit Render(std::shared_ptr<sf::Shape> s,
            sf::Color color = sf::Color::White) noexcept
            : shape(std::move(s)), fillColor(color) {
        }

        /**
         * Crea un componente Render a partir de un tipo de figura.
         *
         * type Tipo de figura (Circle, Rectangle, Triangle).
         * color Color de relleno (por defecto blanco).
         *  Componente Render inicializado con la figura y color.
         */
        [[nodiscard]] static Render Make(ShapeType type,
            sf::Color color = sf::Color::White) {
            std::shared_ptr<sf::Shape> s;
            switch (type) {
            case CIRCLE: {
                auto c = std::make_shared<sf::CircleShape>(50.f);
                c->setOrigin({ 50.f, 50.f });
                s = c;
                break;
            }
            case RECTANGLE: {
                sf::Vector2f size{ 100.f, 50.f };
                auto r = std::make_shared<sf::RectangleShape>(size);
                r->setOrigin(size / 2.f);
                s = r;
                break;
            }
            case TRINAGLE: {
                auto t = std::make_shared<sf::ConvexShape>(3);
                t->setPoint(0, { 0.f, 0.f });
                t->setPoint(1, { 100.f, 0.f });
                t->setPoint(2, { 50.f, 100.f });
                t->setOrigin({ 50.f, 50.f });
                s = t;
                break;
            }
            default:
                break;
            }
            if (s) s->setFillColor(color);
            return Render{ s, color };
        }
    };

} // namespace ECS
