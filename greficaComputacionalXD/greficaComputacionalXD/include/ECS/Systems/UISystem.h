#pragma once
#include "Prerequisitos.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Camera.h"
#include "ECS/Component/Steering.h"
#include "Core/Window.h"
#include <imgui.h>

namespace ECS {

    class Uisystem final : public System {
    public:
        explicit Uisystem(Window& window) noexcept
            : m_window(window) {
        }

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
                        const char* behaviors[] = {
                            "None", "Seek", "Flee", "Arrive",
                            "Wander", "Pursuit", "Obstacle Avoidance", "Waypoint"
                        };
                        ImGui::Combo("Behavior", &behavior, behaviors, IM_ARRAYSIZE(behaviors));
                        s->behavior = static_cast<SteeringType>(behavior);

                        ImGui::SliderFloat2("Target", &s->target.x, 0.f, 800.f);
                        ImGui::SliderFloat("Speed", &s->speed, 10.f, 300.f);
                        ImGui::SliderFloat("Arrive Radius", &s->arriveRadius, 10.f, 200.f);

                        // Nuevo: mostrar índice del waypoint
                        if (s->behavior == SteeringType::WAYPOINT) {
                            ImGui::SliderInt("Current Point", &s->currentPoint, 0, 4);
                            // 0–4 porque tu circuito tiene 5 puntos
                        }

                        ECS::EntityID circleEntity = 0;
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

            // Panel de Opciones Globales de Renderizado
            ImGui::Begin("Render Options");

            // Slider para MSAA
            static int msaaLevel = 0;
            ImGui::SliderInt("MSAA Level", &msaaLevel, 0, 16);
            if (ImGui::Button("Aplicar MSAA")) {
                m_window.setMSAALevel(msaaLevel);
            }

            // Toggle de VSync
            static bool vsync = true;
            if (ImGui::Checkbox("VSync", &vsync)) {
                if (m_window.isOpen()) {
                    m_window.getWindow()->setVerticalSyncEnabled(vsync);
                }
            }

            // Límite de FPS
            static int fpsLimit = 60;
            ImGui::SliderInt("FPS Limit", &fpsLimit, 30, 240);
            if (ImGui::Button("Aplicar FPS Limit")) {
                if (m_window.isOpen()) {
                    m_window.getWindow()->setFramerateLimit(fpsLimit);
                }
            }

            ImGui::End();
        }

    private:
        EntityID selectedEntity = NULL_ENTITY;
        Window& m_window;
    };

}
