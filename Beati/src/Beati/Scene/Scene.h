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

		void DestroyEntity(Entity entity);
		
		void OnUpdate(Timestep delta);
		void OnViewportResize(uint32_t width, uint32_t height);


		template<typename T>
		void OnComponentAdded(Entity entity, T& component) {} // TODO: Specialize for specific components if needed
	private:
		entt::registry m_Registry;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
	
		friend class Entity;
	};
}

