#pragma once
#include "bepch.h"
#include <entt.hpp>

// TODO: Finish Scene class

namespace Beati
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		// virtual void Load() = 0;
		// virtual void Unload() = 0;
	private:
		entt::registry m_Registry;
	};
}

