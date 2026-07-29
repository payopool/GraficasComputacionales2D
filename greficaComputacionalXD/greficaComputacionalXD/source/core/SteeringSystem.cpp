/**
 *  SteeringSystem.cpp
 *  Implementación del sistema de Steering para entidades con movimiento autónomo.
 */

#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"

namespace ECS {

    void SteeringSystem::OnUpdate(Registry& registry, float dt) {
        std::vector<sf::Vector2f> circuitPoints = {
           {100.f, 150.f}, {200.f, 160.f}, {300.f, 180.f}, {400.f, 220.f},
           {450.f, 280.f}, {420.f, 340.f}, {350.f, 380.f}, {250.f, 360.f},
           {150.f, 300.f}, {120.f, 220.f}
        };



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
                    sf::Vector2f desired = target - t.position;
                    float len = std::sqrt(desired.x * desired.x + desired.y * desired.y);
                    if (len > 0.f) desired /= len;

                    sf::Vector2f velocity = desired * s.speed * dt;
                    t.position += velocity;

                    if (len < 10.f) {
                        s.currentPoint = (s.currentPoint + 1) % circuitPoints.size();
                    }
                    break;
                }


                default:
                    break;
                }

                // Actualizar posición de la entidad
                t.position += velocity;
            }
        );
    }

}
