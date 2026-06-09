#pragma once
#include "ECS/ComponentPool.h"

namespace ESC {

    /**
     * @brief Clase View para iterar sobre entidades que poseen un conjunto de componentes.
     *
     * Permite recorrer entidades y aplicar funciones sobre ellas y sus componentes,
     * utilizando el pool más pequeño como referencia para optimizar la iteración.
     */
    template<typename... Components>
    class View {
    public:
        /**
         * @brief Constructor que recibe referencias a pools de componentes.
         * @param pools Pools de componentes a incluir en la vista.
         */
        explicit View(ComponentPool<Components>&... pools) noexcept
            : m_pool(&pools...) {
            FindSmallest();
        }

        /**
         * @brief Itera sobre todas las entidades y aplica una función que recibe entidad y componentes.
         * @tparam Func Tipo de la función.
         * @param func Función a aplicar: `func(EntityID, Component1&, Component2&...)`.
         */
        template<typename Func>
        void Each(Func&& func) {
            if (!m_smallest) return;
            const auto& entities = m_smallest->GetEntities();
            for (std::size_t i = entities.size(); i > 0; --i) {
                const EntityID entity = entities[i - 1];
                if (AllHave(entity)) {
                    std::apply(
                        [&](auto*... pools) {
                            func(entity, pools->Get(entity)...);
                        },
                        m_pool
                    );
                }
            }
        }

        /**
         * @brief Itera sobre todas las entidades y aplica una función que recibe solo la entidad.
         * @tparam Func Tipo de la función.
         * @param func Función a aplicar: `func(EntityID)`.
         */
        template<typename Func>
        void EachEntity(Func&& func) {
            if (!m_smallest) return;
            const auto& entities = m_smallest->GetEntities();
            for (std::size_t i = entities.size(); i > 0; --i) {
                const EntityID entity = entities[i - 1];
                if (AllHave(entity)) {
                    func(entity);
                }
            }
        }

        /**
         * @brief Verifica si la vista está vacía.
         * @return true si no hay entidades, false en caso contrario.
         */
        [[nodiscard]] bool Empty() const noexcept {
            return !m_smallest || m_smallest->Empty();
        }

        /**
         * @brief Obtiene el número de entidades en la vista.
         * @return Cantidad de entidades activas.
         */
        [[nodiscard]] size_t Size() const noexcept {
            return m_smallest ? m_smallest->size() : 0;
        }

    private:
        /**
         * @brief Encuentra el pool más pequeño para optimizar la iteración.
         */
        template<std::size_t I = 0>
        void FindSmallest() noexcept {
            if constexpr (I < sizeof...(Components)) {
                auto* pool = std::get<I>(m_pool);
                if (pool && (!m_smallest || pool->size() < m_smallest->size())) {
                    m_smallest = pool;
                }
                FindSmallest<I + 1>();
            }
        }

        /**
         * @brief Verifica si una entidad existe en todos los pools.
         * @param entity ID de la entidad.
         * @return true si la entidad está en todos los pools, false en caso contrario.
         */
        [[nodiscard]] bool AllHave(EntityID entity) const noexcept {
            return std::apply(
                [entity](auto*... pools) {
                    return (... && (pools && pools->Contains(entity)));
                },
                m_pool
            );
        }

    private:
        /// Tupla de punteros a pools de componentes.
        std::tuple<ComponentPool<Components>*...> m_pool;

        /// Pool más pequeño usado como referencia.
        const SparceSet* m_smallest = nullptr;
    };
}
