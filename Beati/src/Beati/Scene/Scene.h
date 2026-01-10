#pragma once
#include "bepch.h"
#include <entt.hpp>

#include "Beati/Events/Event.h"

#include "Components.h"
#include "Entity.h"
#include "Beati/Core/Timestep.h"


class b2World;

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

		void OnPhysics2DStart();
		void OnPhysics2DStop();
		void OnPhysics2DBodyCreated(Entity entity);

		void OnEvent(Event& e);

		void OnViewportResize(uint32_t width, uint32_t height);


		template<typename T>
		void OnComponentAdded(Entity entity, T& component) {} // TODO: Specialize for specific components if needed

		template<typename ComponentType>
		bool TryGetComponent(entt::registry& registry, entt::entity entity, ComponentType*& outComponent);
	private:
		entt::registry m_Registry;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
	
		friend class Entity;
	};
}

