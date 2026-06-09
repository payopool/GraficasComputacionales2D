#pragma once
#include "Types.h"
#include "Prerequisitos.h"

namespace ECS {

    /**
     * @brief Conjunto disperso (Sparse Set) para gestionar entidades en ECS.
     *
     * Utiliza dos vectores:
     * - `m_sparse`: almacena índices que apuntan a posiciones en `m_dense`.
     * - `m_dense`: almacena los IDs de las entidades activas.
     *
     * Permite operaciones eficientes de inserción, búsqueda y eliminación.
     */
    class SparceSet {
    public:
        /// Constructor por defecto.
        SparceSet() = default;

        /// Destructor virtual por defecto.
        virtual ~SparceSet() = default;

        /**
         * @brief Verifica si una entidad está contenida en el conjunto.
         * @param entity ID de la entidad.
         * @return true si la entidad está presente, false en caso contrario.
         */
        [[nodiscard]] bool Contains(EntityID entity) const noexcept {
            const EntityIndex idx = GetEntityIndex(entity);
            if (idx >= m_sparse.size()) return false;
            const EntityIndex denseIdx = m_sparse[idx];
            return denseIdx < m_dense.size() && m_dense[denseIdx] == entity;
        }

        /**
         * @brief Obtiene el número de entidades en el conjunto.
         * @return Cantidad de entidades activas.
         */
        [[nodiscard]] size_t size() const noexcept {
            return m_dense.size();
        }

        /**
         * @brief Verifica si el conjunto está vacío.
         * @return true si no contiene entidades, false en caso contrario.
         */
        [[nodiscard]] bool Empty() const noexcept {
            return m_dense.empty();
        }

        /**
         * @brief Obtiene todas las entidades activas.
         * @return Referencia constante al vector de entidades.
         */
        [[nodiscard]] const std::vector<EntityID>& GetEntities() const noexcept {
            return m_dense;
        }

        /**
         * @brief Elimina una entidad del conjunto.
         * @param entity ID de la entidad a eliminar.
         */
        virtual void Remove(EntityID entity) {
            if (!Contains(entity)) return;

            const EntityIndex sparseIdx = GetEntityIndex(entity);
            const EntityIndex denseIdx = m_sparse[sparseIdx];
            const EntityID last = m_dense.back();

            // Reemplazar la entidad eliminada con la última
            m_dense[denseIdx] = last;
            m_sparse[GetEntityIndex(last)] = denseIdx;

            m_dense.pop_back();
            m_sparse[sparseIdx] = INVALID;
        }

        /**
         * @brief Limpia todas las entidades del conjunto.
         */
        virtual void Clear() {
            m_sparse.clear();
            m_dense.clear();
        }

    protected:
        /**
         * @brief Inserta una entidad en el conjunto.
         * @param entity ID de la entidad.
         * @return Índice en el vector denso.
         */
        EntityIndex InsertEntity(EntityID entity) {
            const EntityIndex sparseIdx = GetEntityIndex(entity);
            const EntityIndex denseIdx = static_cast<EntityIndex>(m_dense.size());

            if (sparseIdx >= m_sparse.size()) {
                m_sparse.resize(sparseIdx + 1, INVALID);
            }

            assert(m_sparse[sparseIdx] == INVALID && "Entity already exists in the set");

            m_sparse[sparseIdx] = denseIdx;
            m_dense.push_back(entity);

            return denseIdx;
        }

    protected:
        /// Valor inválido para índices.
        static constexpr EntityIndex INVALID = std::numeric_limits<EntityIndex>::max();

        /// Array disperso: mapea índices de entidad a posiciones en `m_dense`.
        std::vector<EntityIndex> m_sparse;

        /// Array denso: contiene los IDs de las entidades activas.
        std::vector<EntityID> m_dense;
    };
}
