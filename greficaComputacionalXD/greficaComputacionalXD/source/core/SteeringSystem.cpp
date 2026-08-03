#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"
#include "Circuit.h"

namespace ECS {

    void SteeringSystem::OnUpdate(Registry& registry, float dt) {
        // pasa tamaño fijo de ventana (ajusta si tu ventana es distinta)
        auto circuitPoints = GetCircuitPoints({ 1280, 720 });

        registry.GetView<Transform, Steering>().Each(
            [&](EntityID id, Transform& t, Steering& s) {
                if (s.behavior == SteeringType::NONE) return;

                sf::Vector2f desired = s.target - t.position;
                float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);
                if (length > 0.f) desired /= length;

                sf::Vector2f velocity;

                switch (s.behavior) {
                case SteeringType::SEEK:
                    velocity = desired * s.speed * dt;
                    break;

                case SteeringType::FLEE:
                    velocity = -desired * s.speed * dt;
                    break;

                case SteeringType::ARRIVE:
                    if (length < s.arriveRadius) {
                        float factor = length / s.arriveRadius;
                        velocity = desired * s.speed * factor * dt;
                    }
                    else {
                        velocity = desired * s.speed * dt;
                    }
                    break;

                case SteeringType::WANDER: {
                    float angle = static_cast<float>(rand()) / RAND_MAX * 5.f * 3.14159f;
                    sf::Vector2f wanderDir(std::cos(angle), std::sin(angle));
                    velocity = wanderDir * s.speed * dt;
                    break;
                }

                case SteeringType::PURSUIT: {
                    sf::Vector2f targetFuture = s.target + s.targetVelocity * dt;
                    sf::Vector2f pursuitDir = targetFuture - t.position;
                    float len = std::sqrt(pursuitDir.x * pursuitDir.x + pursuitDir.y * pursuitDir.y);
                    if (len > 0.f) pursuitDir /= len;
                    velocity = pursuitDir * s.speed * dt;
                    break;
                }

                case SteeringType::OBSTACLE_AVOIDANCE:
                    if (length < s.arriveRadius) {
                        velocity = -desired * s.speed * dt;
                    }
                    break;

                case SteeringType::WAYPOINT: {
                    sf::Vector2f target = circuitPoints[s.currentPoint];
                    desired = target - t.position;   // reutiliza variable
                    float len = std::sqrt(desired.x * desired.x + desired.y * desired.y);
                    if (len > 0.f) desired /= len;

                    velocity = desired * s.speed * dt;   // reutiliza variable
                    t.position += velocity;

                    if (len < 10.f) {
                        s.currentPoint = (s.currentPoint + 1) % circuitPoints.size();
                    }
                    break;
                }

                default:
                    break;
                }

                t.position += velocity;
            }
        );
    }

}
