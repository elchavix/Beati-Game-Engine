#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Beati/Core/UUID.h"
#include "SceneCamera.h"

namespace Beati
{
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		glm::mat4 GetTransform() const
		{
			return Transform;
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};
		
	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
