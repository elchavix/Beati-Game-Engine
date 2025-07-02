#pragma once
#include "Beati/Renderer/RendererAPI.h"

namespace Beati {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		// virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		// virtual void DrawLines(const Scope<VertexArray>& vertexArray, uint32_t vertexCount) override;

		// virtual void BeginScene() override;
		// virtual void EndScene() override;

		// static RendererAPI::API GetAPI() { return RendererAPI::API::OpenGL; }
	};
}

