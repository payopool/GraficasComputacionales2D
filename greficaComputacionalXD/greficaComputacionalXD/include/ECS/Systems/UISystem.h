#pragma once
#include "Prerequisitos.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Camera.h"
#include "ECS/Component/Steering.h"
#include <imgui.h>

namespace ECS {

    /**
     * Uisystem
     * Sistema encargado de mostrar y editar componentes de entidades mediante ImGui.
     *
     * Este sistema renderiza dos paneles principales:
     * - **Outliner**: lista todas las entidades disponibles y permite seleccionar una.
     * - **Inspector**: muestra los componentes de la entidad seleccionada y permite modificarlos.
     *
     * Componentes soportados:
     * - Transform: posición, rotación y escala.
     * - Render: color de relleno.
     * - Steering: comportamiento autónomo (Seek, Flee, Arrive) y parámetros asociados.
     * - Camera: estado activo, zoom y velocidad de seguimiento.
     */
    class Uisystem final : public System {
    public:
        /// Constructor por defecto.
        Uisystem() = default;

        /**
         * Actualiza la interfaz gráfica de usuario.
         *
         * Muestra el panel Outliner con todas las entidades y el panel Inspector
         * con los componentes de la entidad seleccionada. Permite modificar valores
         * en tiempo real mediante controles de ImGui.
         *
         * registry Referencia al registro ECS.
         * deltaTime Tiempo transcurrido entre frames (no usado en este sistema).
         */
        void OnUpdate(Registry& registry, float /*deltaTime*/) override {
            // Panel Outliner
            ImGui::Begin("Figures");
            registry.GetView<Transform>().Each(
                [&](EntityID id, Transform&) {
                    bool isSelected = (selectedEntity == id);
                    if (ImGui::Selectable(("Entity " + std::to_string(id)).c_str(), isSelected)) {
                        selectedEntity = id;
                    }
                }
            );
            ImGui::End();

            // Panel Inspector
            ImGui::Begin("Inspector");

            if (selectedEntity != NULL_ENTITY) {
                // Transform
                if (auto* t = registry.TryGetComponent<Transform>(selectedEntity)) {
                    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::SliderFloat2("Position", &t->position.x, 0.f, 800.f);
                        ImGui::SliderFloat("Rotation", &t->rotation, 0.f, 360.f);
                        ImGui::SliderFloat2("Scale", &t->scale.x, 0.1f, 5.f);
                    }
                }

                // Render
                if (auto* r = registry.TryGetComponent<Render>(selectedEntity)) {
                    if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen)) {
                        float color[3] = {
                            r->fillColor.r / 255.f,
                            r->fillColor.g / 255.f,
                            r->fillColor.b / 255.f
                        };
                        if (ImGui::ColorEdit3("Color", color)) {
                            r->fillColor = sf::Color(
                                static_cast<uint8_t>(color[0] * 255),
                                static_cast<uint8_t>(color[1] * 255),
                                static_cast<uint8_t>(color[2] * 255)
                            );
                        }
                    }
                }

                // Steering
                if (auto* s = registry.TryGetComponent<Steering>(selectedEntity)) {
                    if (ImGui::CollapsingHeader("Steering", ImGuiTreeNodeFlags_DefaultOpen)) {
                        int behavior = static_cast<int>(s->behavior);
                        const char* behaviors[] = { "None", "Seek", "Flee", "Arrive", "Wander", "Pursuit", "Obstacle Avoidance" };
                        ImGui::Combo("Behavior", &behavior, behaviors, IM_ARRAYSIZE(behaviors));
                        s->behavior = static_cast<SteeringType>(behavior);

                        ImGui::SliderFloat2("Target", &s->target.x, 0.f, 800.f);
                        ImGui::SliderFloat("Speed", &s->speed, 10.f, 300.f);
                        ImGui::SliderFloat("Arrive Radius", &s->arriveRadius, 10.f, 200.f);

                        // Ejemplo: si el comportamiento es SEEK, se puede vincular al círculo
                        ECS::EntityID circleEntity = 0; // ID del círculo definido en main
                        if (s->behavior == SteeringType::SEEK) {
                            if (auto* circleTransform = registry.TryGetComponent<Transform>(circleEntity)) {
                                s->target = circleTransform->position;
                            }
                        }
                    }
                }

                // Camera
                if (auto* c = registry.TryGetComponent<Camera>(selectedEntity)) {
                    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::Checkbox("Active", &c->active);
                        ImGui::SliderFloat("Zoom", &c->zoom, 0.5f, 3.f);
                        ImGui::SliderFloat("Follow Speed", &c->followSpeed, 0.1f, 10.f);
                    }
                }
            }

            ImGui::End();
        }

    private:
        EntityID selectedEntity = NULL_ENTITY; ///< Entidad actualmente seleccionada en el Outliner.
    };

}
