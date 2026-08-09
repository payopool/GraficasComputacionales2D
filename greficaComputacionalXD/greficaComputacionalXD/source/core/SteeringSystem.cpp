/**
 *  SteeringSystem.cpp
 *  Sistema ECS encargado de actualizar el movimiento autónomo de las entidades.
 *
 * Implementa diferentes comportamientos de steering:
 * - SEEK: moverse hacia un objetivo.
 * - FLEE: alejarse de un objetivo.
 * - ARRIVE: acercarse desacelerando.
 * - WANDER: movimiento aleatorio.
 * - PURSUIT: perseguir un objetivo en movimiento.
 * - OBSTACLE_AVOIDANCE: evitar obstáculos.
 * - WAYPOINT: seguir puntos de un circuito.
 */

#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"
#include "Circuit.h"

namespace ECS {

    /**
     *  Actualiza el movimiento de las entidades con componente Steering.
     *
     *  registry Registro ECS con todas las entidades y componentes.
     *  dt Tiempo transcurrido desde el último frame (delta time).
     */
    void SteeringSystem::OnUpdate(Registry& registry, float dt) {
        // acumula tiempo
        elapsed += dt;

        // espera antes de arrancar
        if (elapsed < startDelay) {
            return; // no mover competidores todavía
        }

        // Obtener puntos del circuito
        auto circuitPoints = GetCircuitPoints({ 1280, 720 });

        // Iterar sobre todas las entidades con Transform y Steering
        registry.GetView<Transform, Steering>().Each(
            [&](EntityID id, Transform& t, Steering& s) {
                if (s.behavior == SteeringType::NONE) return;

                // Vector hacia el objetivo
                sf::Vector2f desired = s.target - t.position;
                float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);
                if (length > 0.f) desired /= length;

                sf::Vector2f velocity;

                // Selección de comportamiento
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
                    // Seguir puntos del circuito
                    sf::Vector2f target = circuitPoints[s.currentPoint];
                    desired = target - t.position;
                    float len = std::sqrt(desired.x * desired.x + desired.y * desired.y);
                    if (len > 0.f) desired /= len;

                    velocity = desired * s.speed * dt;

                    // Evitar colisiones / simular rebase
                    registry.GetView<Transform, Steering>().Each(
                        [&](EntityID otherId, Transform& ot, Steering& os) {
                            if (id == otherId) return;
                            float dist = std::hypot(ot.position.x - t.position.x,
                                ot.position.y - t.position.y);
                            if (dist < 30.f) {
                                velocity *= 0.8f; // frena si está muy cerca

                                // desplazamiento lateral
                                sf::Vector2f side(-desired.y, desired.x); // vector perpendicular
                                float sideLen = std::sqrt(side.x * side.x + side.y * side.y);
                                if (sideLen > 0.f) side /= sideLen;
                                velocity += side * 15.f * dt; // se mueve un poco al lado
                            }
                            else if (dist > 60.f) {
                                velocity *= 1.1f; // acelera si hay espacio
                            }
                        }
                    );

                    t.position += velocity;

                    // Avanzar al siguiente punto
                    if (len < 10.f) {
                        s.currentPoint = (s.currentPoint + 1) % circuitPoints.size();
                    }
                    break;
                }

                default:
                    break;
                }

                // Actualizar rotación para que apunten hacia adelante
                if (velocity.x != 0.f || velocity.y != 0.f) {
                    t.rotation = std::atan2(velocity.y, velocity.x) * 180.f / 3.14159f;
                }

                // Aplicar movimiento
                t.position += velocity;
            }
        );
    }

}
