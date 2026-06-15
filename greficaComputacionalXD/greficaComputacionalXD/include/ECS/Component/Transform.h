#pragma once
#include "Prerequisitos.h"

namespace ECS {

    /**
     *  Componente de transformación para entidades.
     *
     * Define la posición, rotación y escala de una entidad en el espacio 2D.
     */
    struct Transform {
        /// Posición de la entidad en coordenadas 2D.
        sf::Vector2f position{ 0.f, 0.f };

        /// Rotación de la entidad en grados.
        float rotation{ 0.f };

        /// Escala de la entidad en el eje X e Y.
        sf::Vector2f scale{ 1.f, 1.f };

        /**
         *  Constructor por defecto.
         *
         * Inicializa la posición en (0,0), rotación en 0 y escala en (1,1).
         */
        Transform() = default;

        /**
         *  Constructor parametrizado.
         *
         *  pos Posición inicial.
         * rot Rotación inicial en grados (por defecto 0).
         *  scl Escala inicial (por defecto {1,1}).
         */
        explicit Transform(sf::Vector2f pos,
            float rot = 0.f,
            sf::Vector2f scl = { 1.f, 1.f }) noexcept
            : position(pos), rotation(rot), scale(scl) {
        }

        /**
         *  Traslada la entidad en el espacio 2D.
         *
         *  delta Vector de desplazamiento.
         */
        void Traslate(sf::Vector2f delta) noexcept { position += delta; }

        /**
         *  Rota la entidad.
         *
         *  degrees Grados a añadir a la rotación actual.
         */
        void Rolate(float degrees) noexcept { rotation += degrees; }
    };

} // namespace ECS
