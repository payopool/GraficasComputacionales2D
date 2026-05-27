#pragma once
#include"Prerequisitos.h"
// tipos de fundamentales del entity component system
//enityt id= uin64_t que empaqueta entityidex (posicion array) y version (para evitar colisiones de ids)
// al destruir una entidad, se incrementa su version, por lo que si se vuelve a crear una nueva entidad en la misma posicion del array, tendra un id diferente al anterior

namespace ECS {
	//tipos primitivos
	using EntityIndex = uint32_t;//posicion en el array de entidades
	using EntityVersion = uint32_t;//version de la entidad para evitar colisiones de ids
	using EntityID = uint64_t;//id completo que empaqueta index y version
	using ComponentTypeID = uint32_t;//id de tipo de componente para identificar el tipo de componente en tiempo de ejecucion
	inline constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();//id nulo para representar una entidad no valida
	[[nodiscard]] inline EntityIndex GetEntityIndex(EntityID id) noexcept {
		return
			static_cast<EntityVersion>(id & 0XFFF'FFFFFull);
	}
	[[nodiscard]] inline EntityVersion GetEntityVersion(EntityID id) noexcept {
		return
			static_cast<EntityVersion>((id >> 32) & 0XFFF'FFFFFull);
	}
	[[nodiscard]] inline EntityID MakeEntityID(EntityIndex index, EntityVersion version) noexcept {
		return
			(static_cast<EntityID>(version) << 32) | static_cast<EntityID>(index);
	}
	[[nodiscard]] inline ComponentTypeID NextComponentTypeID() noexcept {
		static ComponentTypeID counter = 0;
		return counter++;
	}
	template<typename T>
	[[nodiscard]] ComponentTypeID GetComponentTypeID() noexcept {
		static const ComponentTypeID ID = NextComponentTypeID();
		return ID;
	}
}