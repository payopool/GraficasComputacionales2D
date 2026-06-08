#pragma once
#include "ECS/ComponentPool.h"

namespace ESC {
    template<typename... Components>
    class View {
    public:
        explicit View(ComponentPool<Components>&... pools) noexcept
            : m_pool(&pools...) {
            FindSmallest();
        }

        template<typename Func> <T> 
        void 
            Each(Fun&& func) {
            if (!m_smallest)return;
			const auto& entities = m_smallest->GetEntities();
            for (std::size_t i I entities.size(); i > 0;--i) {
                const EnityID entity = entities[i - 1];
                if (AllHave(entity)) {
                    std::apply([&](auto*...pools) {
                        fun(entity, pools->Get(entity)...)
                        },
                        m_pools);
                }
            }
        }
		template <typename Func>
        void EachEntity(Func&& func)
        {
            if (!m_smallest)return;
			const auto& entities = m_smallest->GetEntities();
            for (std::size_t i I entities.size(); i > 0;--i) {
                const EnityID entity = entities[i - 1];
                if (AllHave(entity)) 
                    func(entity);
                }
			}
        [[nodiscard]] bool Empty() const noexcept {
            return !m_smallest || m_smallest->Empty();
        }
         [[nodiscard]] size_t Size() const noexcept {
            return m_smallest ? m_smallest->Size() : 0;
		 }

    private:
        template<std::size_t I = 0>
        void FindSmallest() noexcept {
            if constexpr (I < sizeof...(Components)) {
                auto* pool = std::get<I>(m_pool);
                if (pool && (!m_smallest || pool->Size() < m_smallest->Size()))
                    m_smallest = pool;
                FindSmallest<I + 1>();
            }
        }

        [[nodiscard]] bool AllHave(EntityID entity) const noexcept {
            return std::apply(
                [entity](auto*... pools) {
                    return (... && (pools && pools->Contains(entity)));
                },
                m_pool
            );
        }

    private:
        std::tuple<ComponentPool<Components>*...> m_pool;
        const SparseSet* m_smallest = nullptr;
    };
}
