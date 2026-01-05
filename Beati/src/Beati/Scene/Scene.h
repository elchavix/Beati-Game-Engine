#pragma once
#include "bepch.h"
#include <entt.hpp>

#include "Components.h"
#include "Entity.h"
#include "Beati/Core/Timestep.h"

namespace Beati
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		
		void OnUpdate(Timestep delta);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component) {} // TODO: Specialize for specific components if needed
	private:
		entt::registry m_Registry;
	
		friend class Entity;
	};
}

