#pragma once
#include "Prerequisitos.h"

/**
 * @brief Tipos fundamentales del Entity Component System (ECS).
 *
 * - EntityID: empaqueta índice y versión en un único valor de 64 bits.
 * - EntityIndex: posición en el array de entidades.
 * - EntityVersion: versión de la entidad para evitar colisiones de IDs.
 * - ComponentTypeID: identificador único para cada tipo de componente.
 *
 * Al destruir una entidad, se incrementa su versión. Si se crea otra entidad
 * en la misma posición del array, tendrá un ID diferente al anterior.
 */
namespace ECS {

    /// Índice de la entidad dentro del array.
    using EntityIndex = uint32_t;

    /// Versión de la entidad (evita colisiones de IDs).
    using EntityVersion = uint32_t;

    /// ID completo que empaqueta índice y versión.
    using EntityID = uint64_t;

    /// Identificador de tipo de componente.
    using ComponentTypeID = uint32_t;

    /// ID nulo para representar una entidad inválida.
    inline constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

    /**
     * @brief Obtiene el índice de una entidad a partir de su ID.
     * @param id ID completo de la entidad.
     * @return Índice de la entidad.
     */
    [[nodiscard]] inline EntityIndex GetEntityIndex(EntityID id) noexcept {
        return static_cast<EntityIndex>(id & 0xFFF'FFFFFull);
    }

    /**
     * @brief Obtiene la versión de una entidad a partir de su ID.
     * @param id ID completo de la entidad.
     * @return Versión de la entidad.
     */
    [[nodiscard]] inline EntityVersion GetEntityVersion(EntityID id) noexcept {
        return static_cast<EntityVersion>((id >> 32) & 0xFFF'FFFFFull);
    }

    /**
     * @brief Crea un ID de entidad a partir de índice y versión.
     * @param index   Índice de la entidad.
     * @param version Versión de la entidad.
     * @return ID completo de la entidad.
     */
    [[nodiscard]] inline EntityID MakeEntityID(EntityIndex index, EntityVersion version) noexcept {
        return (static_cast<EntityID>(version) << 32) | static_cast<EntityID>(index);
    }

    /**
     * @brief Genera un nuevo identificador de tipo de componente.
     * @return Nuevo ComponentTypeID único.
     */
    [[nodiscard]] inline ComponentTypeID NextComponentTypeID() noexcept {
        static ComponentTypeID counter = 0;
        return counter++;
    }

    /**
     * @brief Obtiene el identificador único de un tipo de componente.
     * @tparam T Tipo de componente.
     * @return ComponentTypeID asociado al tipo T.
     */
    template<typename T>
    [[nodiscard]] ComponentTypeID GetComponentTypeID() noexcept {
        static const ComponentTypeID ID = NextComponentTypeID();
        return ID;
    }
}
