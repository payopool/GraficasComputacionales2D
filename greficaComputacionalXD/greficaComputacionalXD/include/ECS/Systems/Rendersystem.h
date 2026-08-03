#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "core/Window.h"
#include "Circuit.h"
#include <SFML/Graphics.hpp>
#include <cmath>

namespace ECS {

    class RenderSystem final : public System {
    public:
        explicit RenderSystem(Window& window) noexcept
            : m_window(window) {}

        void OnUpdate(Registry& registry, float /*deltaTime*/) override {
            auto circuitPoints = GetCircuitPoints({ 1280, 720 });

            // Pista rellena con grosor
            sf::VertexArray pista(sf::PrimitiveType::TriangleStrip);
            float ancho = 35.f; // ancho moderado

            for (size_t i = 0; i < circuitPoints.size() - 1; ++i) {
                sf::Vector2f p1 = circuitPoints[i];
                sf::Vector2f p2 = circuitPoints[i + 1];

                for (int j = 0; j <= 5; ++j) { // subdividir en 5 pasos
                    float t = j / 5.f;
                    sf::Vector2f p = p1 + t * (p2 - p1);

                    sf::Vector2f dir = p2 - p1;
                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                    if (len == 0) continue;
                    dir /= len;

                    sf::Vector2f normal(-dir.y, dir.x);

                    pista.append({ p + normal * ancho, sf::Color(80, 80, 80) });
                    pista.append({ p - normal * ancho, sf::Color(80, 80, 80) });
                }
            }


            // Cerrar pista conectando al primer punto
            sf::Vector2f first = circuitPoints.front();
            sf::Vector2f second = circuitPoints[1];
            sf::Vector2f dir = second - first;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= len;
            sf::Vector2f normal(-dir.y, dir.x);

            pista.append({ first + normal * ancho, sf::Color(80,80,80) });
            pista.append({ first - normal * ancho, sf::Color(80,80,80) });

            m_window.draw(pista);

            // Línea central amarilla cerrada
            sf::VertexArray circuito(sf::PrimitiveType::LineStrip);
            for (auto& p : circuitPoints) {
                circuito.append({ p, sf::Color::Yellow });
            }
            circuito.append({ circuitPoints.front(), sf::Color::Yellow });
            m_window.draw(circuito);

            // Entidades al frente
            registry.GetView<Transform, Render>().Each(
                [this](EntityID, Transform& t, Render& r) {
                    if (!r.shape || !r.visisble) return;
                    r.shape->setPosition(t.position);
                    r.shape->setRotation(sf::degrees(t.rotation));
                    r.shape->setScale(t.scale);
                    r.shape->setFillColor(r.fillColor);
                    m_window.draw(*r.shape);
                }
            );
        }

    private:
        Window& m_window;
    };

} // namespace ECS
