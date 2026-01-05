#pragma once

#include "Beati/Core/KeyCodes.h"
#include "Beati/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Beati {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
