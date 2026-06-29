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
            ImGui::Begin("Entity Controls");

            // Inspector de entidades con Transform + Render
            registry.GetView<Transform, Render>().Each(
                [&](EntityID id, Transform& t, Render& r) {
                    ImGui::Separator();
                    ImGui::Text("Entity %d", id);

                    ImGui::SliderFloat2(("Position##" + std::to_string(id)).c_str(),
                        &t.position.x, 0.0f, 800.0f);
                    ImGui::SliderFloat(("Rotation##" + std::to_string(id)).c_str(),
                        &t.rotation, 0.0f, 360.0f);
                    ImGui::SliderFloat2(("Scale##" + std::to_string(id)).c_str(),
                        &t.scale.x, 0.1f, 5.0f);

                    float color[3] = {
                        r.fillColor.r / 255.0f,
                        r.fillColor.g / 255.0f,
                        r.fillColor.b / 255.0f
                    };
                    if (ImGui::ColorEdit3(("Color##" + std::to_string(id)).c_str(), color)) {
                        r.fillColor = sf::Color(
                            static_cast<uint8_t>(color[0] * 255),
                            static_cast<uint8_t>(color[1] * 255),
                            static_cast<uint8_t>(color[2] * 255)
                        );
                    }
                }
            );

            // Inspector de cámaras
            registry.GetView<Transform, Camera>().Each(
                [&](EntityID id, Transform& t, Camera& c) {
                    ImGui::Separator();
                    ImGui::Text("Camera %d", id);

                    ImGui::Checkbox(("Active##" + std::to_string(id)).c_str(), &c.active);
                    ImGui::SliderFloat2(("Position##Cam" + std::to_string(id)).c_str(),
                        &t.position.x, 0.0f, 800.0f);
                    ImGui::SliderFloat(("Zoom##" + std::to_string(id)).c_str(),
                        &c.zoom, 0.5f, 3.0f);
                    ImGui::SliderFloat(("FollowSpeed##" + std::to_string(id)).c_str(),
                        &c.followSpeed, 0.1f, 10.0f);
                }
            );

            ImGui::End();
        }
    };
}
