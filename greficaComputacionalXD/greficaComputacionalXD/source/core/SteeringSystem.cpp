#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"            
#include "ECS/Component/Transform.h"   
#include "ECS/Component/Steering.h"    
                  
namespace ECS {

    void SteeringSystem ::OnUpdate(Registry& registry, float dt) {
        registry.GetView<Transform, Steering>().Each(
            [&](EntityID id, Transform& t, Steering& s) {
                if (s.behavior == SteeringType::NONE) return;

                // Vector deseado hacia el objetivo
                sf::Vector2f desired = s.target - t.position;
                float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);

                if (length > 0.f) {
                    desired /= length; // normalizar
                }

                sf::Vector2f velocity;

                switch (s.behavior) {
                case SteeringType::SEEK:
                    /// Movimiento hacia el objetivo
                    velocity = desired * s.speed * dt;
                    break;

                case SteeringType::FLEE:
                    /// Movimiento alejándose del objetivo
                    velocity = -desired * s.speed * dt;
                    break;

                case SteeringType::ARRIVE:
                    /// Movimiento hacia el objetivo con desaceleración
                    if (length < s.arriveRadius) {
                        float factor = length / s.arriveRadius;
                        velocity = desired * s.speed * factor * dt;
                    }
                    else {
                        velocity = desired * s.speed * dt;
                    }
                    break;

                case SteeringType::WANDER: {
                    // Genera un desplazamiento aleatorio
                    float angle = static_cast<float>(rand()) / RAND_MAX * 5.f * 3.14159f;
                    sf::Vector2f wanderDir(std::cos(angle), std::sin(angle));
                    velocity = wanderDir * s.speed * dt;
                    break;
                }

                case SteeringType::PURSUIT: {
                    // Persigue a otra entidad (ejemplo: círculo)
                    // Aquí necesitarías obtener la posición y velocidad del objetivo
                    sf::Vector2f targetFuture = s.target + s.targetVelocity * dt;
                    sf::Vector2f pursuitDir = targetFuture - t.position;
                    float len = std::sqrt(pursuitDir.x * pursuitDir.x + pursuitDir.y * pursuitDir.y);
                    if (len > 0.f) pursuitDir /= len;
                    velocity = pursuitDir * s.speed * dt;
                    break;
                }

                case SteeringType::OBSTACLE_AVOIDANCE: {
                    // Evita obstáculos (simplificado: si está muy cerca, se mueve en dirección opuesta)
                    if (length < s.arriveRadius) {
                        velocity = -desired * s.speed * dt;
                    }
                    break;
                }

                default:
                    break;
                }

                // Actualizar posición
                t.position += velocity;
            }
        );
    }

}
