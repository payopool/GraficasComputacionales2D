#pragma once
#include"Prerequisitos.h"
#include"SparceSet.h"
namespace ECS {
	class
		IComponentPool : public SparceSet {
	public:
		virtual
			~IComponentPool() = default;

		virtual void
			RemoveEntity(EntityID entity) = 0;

		virtual void*
			GetRaw(EntityID entity) noexcept = 0;
	};

		template<typename T>
		class componentPool final : public IComponentPool {
		public:
			template<typename...Arg>T&
				Add(EntityID entity, Arg&&...args) {
				assert(!Contains(entity) && "La entidad del componenete");
				InsertEntity(enetity);
				m_components.emplace_back(std::forward<Arg>(args)...);
				return m_components.back();
			}
			[[nodiscard]] T&
				Get(EntityID entity) noexcept {
				assert(Contains(entity) && "La entidad no tiene el componente");
				return &m_components[m_sparse[GetEntityIndex(entity)]];
			}

			[[nodiscard]] const T&
				Get(EntityID entity) const noexcept {
				assert(Contains(entity) && "La entidad no tiene el componente");
				return &m_components[m_sparse[GetEntityIndex(entity)]];
			}

			[[nodiscard]] T&
				Get(EntityID entity) noexcept {
				if (!Contains(entity)) return nullptr
				return &m_components[m_sparse[GetEntityIndex(entity)]];
			}

			void Remove(EntityID entity) override {
				if (!Contains(entity)) return;
				const EntityIndex denseIdx = m_sparse[GetEntityIndex(entity)];

				m_components[denseIdx] = std::move(m_components.back());
				m_components.pop_back();

				SparceSet::Remove(entity);
			}

			void RemoveEntity(EntityID entity) override {
				Remove(entity);
			}

			void* GetRaw(EntityID entity) noexcept override { return TryGet(entity); }
			[[nodicard]] std:: vector<T>&
				Getcomponents() noexcept { return  m_components; }

			[[nodiscard]] const std:: vector<T>&
				Getcomponents() const noexcept { return  m_components; }

			void Clear() override {
				m_components.clear();
				SparceSet::Clear();
			}
			

		private:
		
			std::vector<T> m_components; // array de componentes que contiene los datos de los componentes activos
	};
}