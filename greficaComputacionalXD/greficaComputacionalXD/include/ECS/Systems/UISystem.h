#pragma once
#include "Prerequisitos.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Render.h"
#include "ECS/Component/Camera.h"
#include <imgui.h>

namespace ECS {
    class Uisystem final : public System {
    public:
        Uisystem() = default;

        


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
        EntityID selectedEntity = NULL_ENTITY; 
    };
}
