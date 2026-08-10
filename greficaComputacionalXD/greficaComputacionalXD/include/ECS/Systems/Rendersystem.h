/**
 *  RenderSystem.h
 *  Sistema ECS encargado de dibujar la pista, meta y entidades en pantalla.
 *
 * Este sistema:
 * - Dibuja el fondo (césped).
 * - Renderiza la pista gris con grosor.
 * - Traza la línea central amarilla.
 * - Dibuja la meta como cuadros blancos y negros.
 * - Renderiza las entidades con componentes Transform y Render.
 */

#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Meta.h"
#include "core/Window.h"
#include "Circuit.h"

namespace ECS {

    class RenderSystem final : public System {
    public:
        explicit RenderSystem(Window& window) noexcept
            : m_window(window) {
        }

        void OnUpdate(Registry& registry, float /*deltaTime*/) override {
            auto circuitPoints = GetCircuitPoints({ 1280, 720 });

            // Fondo verde
            sf::RectangleShape fondo(sf::Vector2f(1280, 720));
            fondo.setFillColor(sf::Color(0, 150, 0));
            m_window.draw(fondo);

            // Pista gris con grosor
            sf::VertexArray pista(sf::PrimitiveType::TriangleStrip);
            float ancho = 30.f;

            for (size_t i = 0; i < circuitPoints.size() - 1; ++i) {
                sf::Vector2f p1 = circuitPoints[i];
                sf::Vector2f p2 = circuitPoints[i + 1];

                for (int j = 0; j <= 20; ++j) { // más subdivisiones para suavizar
                    float t = j / 20.f;
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

            // cerrar pista
            sf::Vector2f first = circuitPoints.front();
            sf::Vector2f second = circuitPoints[1];
            sf::Vector2f dir = second - first;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            dir /= len;
            sf::Vector2f normal(-dir.y, dir.x);

            pista.append({ first + normal * ancho, sf::Color(80,80,80) });
            pista.append({ first - normal * ancho, sf::Color(80,80,80) });

            m_window.draw(pista);

            // Línea central amarilla
            sf::VertexArray circuito(sf::PrimitiveType::LineStrip);
            for (auto& p : circuitPoints) {
                circuito.append({ p, sf::Color::Yellow });
            }
            circuito.append({ circuitPoints.front(), sf::Color::Yellow });
            m_window.draw(circuito);

            // --- Dibujar meta ---
            registry.GetView<Meta>().Each([&](EntityID, Meta& meta) {
                size_t metaIndex = meta.index;
                if (metaIndex >= circuitPoints.size() - 1) metaIndex = circuitPoints.size() - 2;

                sf::Vector2f metaPos = circuitPoints[metaIndex];
                sf::Vector2f nextPos = circuitPoints[(metaIndex + 1) % circuitPoints.size()];

                // dirección del tramo
                sf::Vector2f metaDir = nextPos - metaPos;
                float metaLen = std::sqrt(metaDir.x * metaDir.x + metaDir.y * metaDir.y);
                if (metaLen != 0) metaDir /= metaLen;

                // normal perpendicular
                sf::Vector2f metaNormal(-metaDir.y, metaDir.x);

                // ángulo en grados
                float angleDeg = std::atan2(metaDir.y, metaDir.x) * 180.f / 3.14159f;

                // ancho de la pista (igual que arriba)
                float pistaAncho = ancho;

                // meta del mismo ancho que la pista
                float cuadroAncho = 20.f;             // ancho de cada cuadro
                float cuadroAlto = pistaAncho * 2.f; // cruza de borde a borde

                // base de la meta: centrada en la pista
                sf::Vector2f basePos = metaPos - metaNormal * (cuadroAlto / 2.f);

                for (int i = 0; i < 8; ++i) {
                    sf::RectangleShape cuadro(sf::Vector2f(cuadroAncho, cuadroAlto / 8.f));
                    cuadro.setPosition(basePos + metaNormal * (i * (cuadroAlto / 8.f)));
                    cuadro.setRotation(sf::degrees(angleDeg)); // SFML 3
                    cuadro.setFillColor((i % 2 == 0) ? sf::Color::White : sf::Color::Black);
                    m_window.draw(cuadro);
                }



                });

            registry.GetView<Transform, Render>().Each(
                [this](EntityID id, Transform& t, Render& r) {
                    if (!r.shape || !r.visisble) return;

                    // --- Depuración: imprimir coordenadas ---
                    std::cout << "Carro " << id << " en ("
                        << t.position.x << ", " << t.position.y
                        << ") rot=" << t.rotation << std::endl;

                    // --- Depuración: dibujar punto rojo en el centro del carro ---
                    sf::CircleShape debugPoint(5.f);
                    debugPoint.setFillColor(sf::Color::Red);
                    debugPoint.setPosition(t.position);
                    m_window.draw(debugPoint);

                    // Render normal del carro
                    r.shape->setPosition(t.position);
                    r.shape->setRotation(sf::degrees(t.rotation)); // SFML 3
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
