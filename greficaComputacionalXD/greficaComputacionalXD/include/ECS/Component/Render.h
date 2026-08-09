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

        std::shared_ptr<sf::Texture> texture;

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
        bool SetTexture(const std::string& path,bool resetRect = true
        ){
            if (!shape)return false;
            auto tex = std::make_shared<sf::Texture>();
            if (!tex->loadFromFile(path))return false;
            texture = std::move(tex);
            shape->setTexture(texture.get(), resetRect);
            return true;
        }
        void SetTexture(std::shared_ptr<sf::Texture> tex, bool resetRect = true) {
            if (!shape)return;
            texture = std::move(tex);
            shape->setTexture(texture ? texture.get() : nullptr, resetRect);
		}
        void ClearTexture(){
            if (!shape)  shape->setTexture(nullptr);
            texture.reset();
           
		}
        /**
         * Crea un componente Render a partir de un tipo de figura.
         *
         * type Tipo de figura (Circle, Rectangle, Triangle).
         * color Color de relleno (por defecto blanco).
         *  Componente Render inicializado con la figura y color.
         */
        [[nodiscard]] static Render Make(
            ShapeType type,
            sf::Color color = sf::Color::White,
            const std::string& texturePath = ""
        ) {
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
            case CAR: {
                sf::Vector2f size{ 60.f, 30.f }; // antes era 40x20, ahora el doble
                auto r = std::make_shared<sf::RectangleShape>(size);
                r->setOrigin(size / 2.f); // centro para que rote bien
                s = r;
                break;
            }

            default:
                break;
            }

            Render render{ s, color };

            if (!texturePath.empty()) {
                render.SetTexture(texturePath);
            }

            if (s) s->setFillColor(color);
            return render;
        }



    };

} // namespace ECS
