#pragma once
#include "Beati/Renderer/OrthographicCamera.h"
#include <glm/glm.hpp>

#include <Platform/OpenGL/OpenGLTexture.h>

namespace Beati {

	struct RendererStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t TextureCount = 0;

		void Reset()
		{
			DrawCalls = 0;
			QuadCount = 0;
			TextureCount = 0;
		}
	};
	
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Beati::Texture2D>& texture, float tilingFactor = 1.0f);

		// Text
		static void DrawString(const std::string& text, const glm::vec3& position, float size, const glm::vec4& color);

		// Stats
		static RendererStats GetStats();
	};
}

