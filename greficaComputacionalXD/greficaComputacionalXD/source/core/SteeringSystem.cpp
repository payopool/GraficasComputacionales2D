/**
 *  SteeringSystem.cpp
 *  Implementación del sistema de Steering para entidades con movimiento autónomo.
 *
 * Este sistema aplica diferentes comportamientos de movimiento (Seek, Flee, Arrive,
 * Wander, Pursuit, Obstacle Avoidance) sobre las entidades que poseen los componentes
 * Transform y Steering. Cada comportamiento define cómo la entidad ajusta su posición
 * en relación con un objetivo o con su entorno.
 */

#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Component/Transform.h"
#include "ECS/Component/Steering.h"
#include <cmath>
#include <cstdlib>

namespace ECS {

    /**
     *  Actualiza las posiciones de las entidades según su comportamiento Steering.
     *
     * Recorre todas las entidades que poseen los componentes Transform y Steering,
     * calcula un vector de movimiento en función del comportamiento seleccionado,
     * y actualiza la posición de la entidad.
     *
     *  registry Referencia al registro ECS.
     *  dt Delta time (tiempo transcurrido entre frames).
     */
    void SteeringSystem::OnUpdate(Registry& registry, float dt) {
        registry.GetView<Transform, Steering>().Each(
            [&](EntityID id, Transform& t, Steering& s) {
                if (s.behavior == SteeringType::NONE) return;

                // Vector deseado hacia el objetivo
                sf::Vector2f desired = s.target - t.position;
                float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);

                if (length > 0.f) {
                    desired /= length; ///< Normalizar vector
                }

                sf::Vector2f velocity;

                switch (s.behavior) {
                case SteeringType::SEEK:
                    /**
                     *  SEEK: Movimiento directo hacia el objetivo.
                     *
                     * La entidad se desplaza en dirección al target sin desaceleración.
                     */
                    velocity = desired * s.speed * dt;
                    break;

                case SteeringType::FLEE:
                    /**
                     *  FLEE: Movimiento alejándose del objetivo.
                     *
                     * La entidad se desplaza en dirección opuesta al target.
                     */
                    velocity = -desired * s.speed * dt;
                    break;

                case SteeringType::ARRIVE:
                    /**
                     *  ARRIVE: Movimiento hacia el objetivo con desaceleración.
                     *
                     * La entidad reduce su velocidad al acercarse al target dentro
                     * de un radio definido (arriveRadius).
                     */
                    if (length < s.arriveRadius) {
                        float factor = length / s.arriveRadius;
                        velocity = desired * s.speed * factor * dt;
                    }
                    else {
                        velocity = desired * s.speed * dt;
                    }
                    break;

                case SteeringType::WANDER: {
                    /**
                     *  WANDER: Movimiento errático en direcciones aleatorias.
                     *
                     * La entidad genera un vector aleatorio cada frame, simulando
                     * un patrullaje o desplazamiento sin rumbo fijo.
                     */
                    float angle = static_cast<float>(rand()) / RAND_MAX * 5.f * 3.14159f;
                    sf::Vector2f wanderDir(std::cos(angle), std::sin(angle));
                    velocity = wanderDir * s.speed * dt;
                    break;
                }

                case SteeringType::PURSUIT: {
                    /**
                     *  PURSUIT: Persecución de un objetivo en movimiento.
                     *
                     * La entidad intenta interceptar al target prediciendo su posición futura
                     * a partir de su velocidad. Si no se define targetVelocity, funciona como SEEK.
                     */
                    sf::Vector2f targetFuture = s.target + s.targetVelocity * dt;
                    sf::Vector2f pursuitDir = targetFuture - t.position;
                    float len = std::sqrt(pursuitDir.x * pursuitDir.x + pursuitDir.y * pursuitDir.y);
                    if (len > 0.f) pursuitDir /= len;
                    velocity = pursuitDir * s.speed * dt;
                    break;
                }

                case SteeringType::OBSTACLE_AVOIDANCE: {
                    /**
                     *  OBSTACLE AVOIDANCE: Evitación de obstáculos.
                     *
                     * Si la entidad se encuentra demasiado cerca del objetivo (dentro del arriveRadius),
                     * se desplaza en dirección opuesta para evitar colisión.
                     */
                    if (length < s.arriveRadius) {
                        velocity = -desired * s.speed * dt;
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
