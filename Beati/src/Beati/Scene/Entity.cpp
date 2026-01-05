#include "bepch.h"
#include "Entity.h"

namespace Beati {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}
