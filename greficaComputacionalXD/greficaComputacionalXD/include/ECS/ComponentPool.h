#pragma once
#include "Types.h"
#include "Prerequisitos.h"
#include "SparceSet.h"

namespace ECS {

    /**
     * @brief Interfaz base para pools de componentes.
     *
     * Hereda de SparceSet y define las operaciones mínimas
     * que cualquier pool de componentes debe implementar.
     */
    class IComponentPool : public SparceSet {
    public:
        virtual ~IComponentPool() = default;

        /**
         * @brief Elimina una entidad del pool.
         * @param entity ID de la entidad.
         */
        virtual void RemoveEntity(EntityID entity) = 0;

        /**
         * @brief Obtiene un puntero crudo al componente asociado a la entidad.
         * @param entity ID de la entidad.
         * @return Puntero al componente, o nullptr si no existe.
         */
        virtual void* GetRaw(EntityID entity) noexcept = 0;
    };

    /**
     * @brief Pool de componentes para un tipo específico T.
     *
     * Gestiona la creación, acceso y eliminación de componentes
     * asociados a entidades dentro del ECS.
     */
    template<typename T>
    class componentPool final : public IComponentPool {
    public:
        /**
         * @brief Añade un componente a una entidad.
         * @param entity ID de la entidad.
         * @param args   Argumentos para construir el componente.
         * @return Referencia al componente creado.
         */
        template<typename... Arg>
        T& Add(EntityID entity, Arg&&... args) {
            assert(!Contains(entity) && "La entidad ya tiene el componente");
            InsertEntity(entity);
            m_components.emplace_back(std::forward<Arg>(args)...);
            return m_components.back();
        }

        /**
         * @brief Obtiene un componente asociado a una entidad.
         * @param entity ID de la entidad.
         * @return Referencia al componente.
         */
        [[nodiscard]] T& Get(EntityID entity) noexcept {
            assert(Contains(entity) && "La entidad no tiene el componente");
            return m_components[m_sparse[GetEntityIndex(entity)]];
        }

        /**
         * @brief Obtiene un componente asociado a una entidad (const).
         * @param entity ID de la entidad.
         * @return Referencia constante al componente.
         */
        [[nodiscard]] const T& Get(EntityID entity) const noexcept {
            assert(Contains(entity) && "La entidad no tiene el componente");
            return m_components[m_sparse[GetEntityIndex(entity)]];
        }

        /**
         * @brief Intenta obtener un componente asociado a una entidad.
         * @param entity ID de la entidad.
         * @return Puntero al componente, o nullptr si no existe.
         */
        [[nodiscard]] T* TryGet(EntityID entity) noexcept {
            if (!Contains(entity)) return nullptr;
            return &m_components[m_sparse[GetEntityIndex(entity)]];
        }

        /**
         * @brief Elimina un componente asociado a una entidad.
         * @param entity ID de la entidad.
         */
        void Remove(EntityID entity) override {
            if (!Contains(entity)) return;
            const EntityIndex denseIdx = m_sparse[GetEntityIndex(entity)];

            // Reemplazar el componente eliminado con el último
            m_components[denseIdx] = std::move(m_components.back());
            m_components.pop_back();

            SparceSet::Remove(entity);
        }

        /**
         * @brief Elimina una entidad del pool.
         * @param entity ID de la entidad.
         */
        void RemoveEntity(EntityID entity) override {
            Remove(entity);
        }

        /**
         * @brief Obtiene un puntero crudo al componente asociado a la entidad.
         * @param entity ID de la entidad.
         * @return Puntero al componente, o nullptr si no existe.
         */
        void* GetRaw(EntityID entity) noexcept override {
            return TryGet(entity);
        }

        /**
         * @brief Obtiene todos los componentes activos.
         * @return Referencia al vector de componentes.
         */
        [[nodiscard]] std::vector<T>& GetComponents() noexcept {
            return m_components;
        }

        /**
         * @brief Obtiene todos los componentes activos (const).
         * @return Referencia constante al vector de componentes.
         */
        [[nodiscard]] const std::vector<T>& GetComponents() const noexcept {
            return m_components;
        }

        /**
         * @brief Limpia todos los componentes del pool.
         */
        void Clear() override {
            m_components.clear();
            SparceSet::Clear();
        }

    private:
        /// Array de componentes que contiene los datos de los componentes activos.
        std::vector<T> m_components;
    };
}
