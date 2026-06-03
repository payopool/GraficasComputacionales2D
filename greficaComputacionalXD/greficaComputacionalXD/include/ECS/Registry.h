#pragma once
#include"ComponentPool.h"
#include"View.h"
#include"Types.h"
#include"System.h"
namespace ECS {
	class Registry {
	private:
		// entidades 
		std::vector<EntityID> m_entities;
		std::vector<EntityVersion> m_versions;
		std::vector<EntityIndex> m_freeIndices;
		// componenetes
		std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>> m_componentPools;
		// sistemas
		std::vector < std::unique_ptr<System> m_systems;
	};
}