#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "core/Window.h"

namespace ECS {

    /**
     *Sistema encargado de renderizar entidades con componentes Transform y Render.
     *
     * Itera sobre todas las entidades que poseen ambos componentes y las dibuja en la ventana.
     */
    class RenderSystem final : public System {
    public:
        /**
         * Constructor del sistema de renderizado.
         * window Referencia a la ventana donde se dibujarán las entidades.
         */
        explicit RenderSystem(Window& window) noexcept
            : m_window(window) {
        }

        /**
         * Actualiza el sistema de renderizado.
         *
         * Recorre todas las entidades con Transform y Render, aplica sus transformaciones
         * y dibuja las figuras en la ventana.
         *
         * registry Registro ECS que contiene entidades y componentes.
         * deltaTime Tiempo transcurrido desde el último frame (no usado aquí).
         */
        void OnUpdate(Registry& registry, float /*deltaTime*/) override {
            registry.GetView<Transform, Render>().Each(
                [this](EntityID, Transform& t, Render& r) {
                    if (!r.shape || !r.visisble) return;

                    // Aplicar transformacionesS
                    r.shape->setPosition(t.position);
                    r.shape->setRotation(sf::degrees(t.rotation));
                    r.shape->setScale(t.scale);
                    r.shape->setFillColor(r.fillColor);
                    // Dibujar en la ventana
                    m_window.draw(*r.shape);
                }
            );
        }

    private:
        /// Referencia a la ventana principal.
        Window& m_window;
    };

} // namespace ECS
