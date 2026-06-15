#pragma once
#include "ComponentPool.h"
#include "View.h"
#include "Types.h"
#include "System.h"

namespace ECS {

    /**
     * @brief Registro principal del ECS.
     *
     * Gestiona la creación y destrucción de entidades, así como la
     * administración de componentes y sistemas.
     */
    class Registry {
    public:
        /**
         * @brief Crea una nueva entidad.
         * @return ID de la entidad creada.
         */
        EntityID CreateEntity();

        /**
         * @brief Destruye una entidad y elimina sus componentes.
         * @param entity ID de la entidad a destruir.
         */
        void DestroyEntity(EntityID entity);

        /**
         * @brief Verifica si una entidad está viva.
         * @param entity ID de la entidad.
         * @return true si la entidad está activa, false en caso contrario.
         */
        [[nodiscard]] bool IsAlive(EntityID entity) const noexcept;

        /**
         * @brief Obtiene el número de entidades activas.
         * @return Cantidad de entidades.
         */
        [[nodiscard]] std::size_t EntityCount() const noexcept;

        /**
         * @brief Devuelve todas las entidades activas.
         * @return Referencia constante al vector de entidades.
         */
        [[nodiscard]] const std::vector<EntityID>& GetEntities() const noexcept;

        /**
         * @brief Añade un componente a una entidad.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @param args Argumentos para construir el componente.
         * @return Referencia al componente creado.
         */
        template<typename T, typename... Args>
        T& AddComponent(EntityID entity, Args&&... args);

        /**
         * @brief Elimina un componente de una entidad.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         */
        template<typename T>
        void RemoveComponent(EntityID entity);

        /**
         * @brief Establece un componente en una entidad.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @param value Valor del componente.
         * @return Referencia al componente.
         */
        template<typename T>
        T& SetComponent(EntityID entity, T value);

        /**
         * @brief Verifica si una entidad tiene un componente.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @return true si la entidad tiene el componente.
         */
        template<typename T>
        [[nodiscard]] bool HasComponent(EntityID entity) const noexcept;

        /**
         * @brief Obtiene un componente de una entidad.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @return Referencia al componente.
         */
        template<typename T>
        [[nodiscard]] T& GetComponent(EntityID entity);

        /**
         * @brief Obtiene un componente de una entidad (const).
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @return Referencia constante al componente.
         */
        template<typename T>
        [[nodiscard]] const T& GetComponent(EntityID entity) const;

        /**
         * @brief Intenta obtener un componente de una entidad.
         * @tparam T Tipo de componente.
         * @param entity ID de la entidad.
         * @return Puntero al componente o nullptr si no existe.
         */
        template<typename T>
        [[nodiscard]] T* TryGetComponent(EntityID entity) noexcept;

        /**
         * @brief Crea una vista sobre entidades que poseen ciertos componentes.
         *
         * Permite iterar sobre todas las entidades que tengan los componentes
         * especificados en la plantilla.
         *
         * @tparam Components Lista de tipos de componentes.
         * @return Vista que contiene las entidades con dichos componentes.
         *
         * @code
         * auto view = registry.GetView<Transform, Render>();
         * for (auto entity : view) {
         *     // trabajar con Transform y Render
         * }
         * @endcode
         */
        template<typename... Components>
        [[nodiscard]] View<Components...> GetView() {
            return View<Components...>(GetOrCreatePool<Components>()...);
        }

        /**
         * @brief Añade un sistema al registro.
         * @tparam T Tipo de sistema (debe derivar de System).
         * @param args Argumentos para construir el sistema.
         * @return Referencia al sistema creado.
         */
        template<typename T, typename... Args>
        T& AddSystem(Args&&... args);

        /**
         * @brief Actualiza todos los sistemas habilitados.
         * @param deltaTime Tiempo transcurrido desde el último frame.
         */
        void UpdateSystems(float deltaTime);

        /**
         * @brief Elimina todos los sistemas.
         */
        void RemoveAllSystems();

        /**
         * @brief Limpia todas las entidades, componentes y sistemas.
         */
        void Clear();

        /**
         * @brief Devuelve todos los pools de componentes.
         * @return Mapa de pools de componentes.
         */
        [[nodiscard]] const std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>>&
            GetPools() const noexcept;

    private:
        /**
         * @brief Obtiene o crea un pool de componentes.
         * @tparam T Tipo de componente.
         * @return Puntero al pool.
         */
        template<typename T>
        componentPool<T>* GetOrCreatePool();

        /**
         * @brief Obtiene un pool de componentes.
         * @tparam T Tipo de componente.
         * @return Puntero al pool o nullptr si no existe.
         */
        template<typename T>
        componentPool<T>* GetPool() noexcept;

        /**
         * @brief Obtiene un pool de componentes (const).
         * @tparam T Tipo de componente.
         * @return Puntero constante al pool o nullptr si no existe.
         */
        template<typename T>
        const componentPool<T>* GetPoolConst() const noexcept;

    private:
        /// Array de entidades activas.
        std::vector<EntityID> m_entities;

        /// Array de versiones de entidades.
        std::vector<EntityVersion> m_versions;

        /// Lista de índices libres para reutilizar.
        std::queue<EntityIndex> m_freeList;

        /// Pools de componentes.
        std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>> m_componentPools;

        /// Sistemas registrados.
        std::vector<std::unique_ptr<System>> m_systems;
    };

} // namespace ECS

