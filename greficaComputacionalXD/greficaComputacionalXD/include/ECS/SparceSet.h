#pragma once
#include"Types.h"
#include"Prerequisitos.h"
namespace ECS {
    class SparceSet {
    public:
        
        SparceSet() = default;
        virtual ~SparceSet() = default;

        [[nodiscard]] bool Contains(EntityID entity) const noexcept {
            const EntityIndex idx = GetEntityIndex(entity);
            if (idx >= m_sparse.size()) return false;
            const EntityIndex denseidx = m_sparse[idx];
            return denseidx < m_dense.size() && m_dense[denseidx] == entity;
        }

        [[nodiscard]] size_t size() const noexcept {
            return m_dense.size();
        }

        [[nodiscard]] bool Empty() const noexcept {
            return m_dense.empty();
        }

        [[nodiscard]] const std::vector<EntityID>& GetEntities() const noexcept {
            return m_dense;
        }

        virtual void Remove(EntityID entity) {
            if (!Contains(entity)) return;
            const EntityIndex sparceIdx = GetEntityIndex(entity);
            const EntityIndex denseIdx = m_sparse[sparceIdx];
            const EntityID last = m_dense.back();

            m_dense[denseIdx] = last;
            m_sparse[GetEntityIndex(last)] = denseIdx;

            m_dense.pop_back();
            m_sparse[sparceIdx] = INVALID;
        }

        virtual void Clear() {
            m_sparse.clear();
            m_dense.clear();
        }

    protected:
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
        static constexpr EntityIndex INVALID = std::numeric_limits<EntityIndex>::max();
        std::vector<EntityIndex> m_sparse; // array de indices que apunta a la posicion del array de entidades
        std::vector<EntityID> m_dense;     // array de entidades que contiene los ids de las entidades activas
    };
}