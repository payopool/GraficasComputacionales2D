#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "core/Window.h"
#include <SFML/Graphics.hpp>

namespace ECS {

    class RenderSystem final : public System {
    public:
        explicit RenderSystem(Window& window) noexcept
            : m_window(window) {
        }

        void OnUpdate(Registry& registry, float /*deltaTime*/) override {
            // Dibujar entidades normales
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

            // Puntos del circuito (ajústalos para que sigan la pista de tu imagen)
            // RenderSystem.cpp
            std::vector<sf::Vector2f> circuitPoints = {
                {100.f, 150.f}, {200.f, 160.f}, {300.f, 180.f}, {400.f, 220.f},
                {450.f, 280.f}, {420.f, 340.f}, {350.f, 380.f}, {250.f, 360.f},
                {150.f, 300.f}, {120.f, 220.f}
            };

            // Dibujar línea amarilla que conecta todos los puntos
            sf::VertexArray circuito(sf::PrimitiveType::LineStrip);
            for (auto& p : circuitPoints) {
                circuito.append({ p, sf::Color::Yellow });
            }
            // cerrar el circuito
            circuito.append({ circuitPoints.front(), sf::Color::Yellow });

            m_window.draw(circuito);

        }

    private:
        Window& m_window;
    };

} // namespace ECS
