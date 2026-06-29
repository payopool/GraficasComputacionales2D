#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"          
#include "ECS/Component/Transform.h"
#include "ECS/Component/Camera.h"
#include "Core/Window.h"
#include <cmath>

namespace ECS {
    class CameraSystem final : public System {
    public:
        explicit CameraSystem(Window& window) noexcept
            : m_window(window) {
        }

        void OnUpdate(Registry& registry, float deltatime) override {
            registry.GetView<Transform, Camera>().Each(
                [this, &registry, deltatime](EntityID, Transform& camT, Camera& cam) {
                    if (!cam.active) return;

                    if (cam.followTarget != NULL_ENTITY &&
                        registry.IsAlive(cam.followTarget)) {
                        if (auto* targetT = registry.TryGetComponent<Transform>(cam.followTarget)) {
                            const float t = 1.f - std::exp(-cam.followSpeed * deltatime);
                            camT.position += (targetT->position - camT.position) * t;
                        }
                    }

                    m_window.applyCameraView(camT.position, cam.zoom);
                });
        }

    private:
        Window& m_window;
    };
}
