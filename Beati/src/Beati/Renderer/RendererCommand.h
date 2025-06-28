#pragma once

#include "RendererAPI.h"

namespace Beati {

	class RendererCommand
	{
	public:
		// inline static void Init() { s_RendererAPI->Init(); }

		//inline static void SetViewport(int x, int y, int width, int height) { s_RendererAPI->SetViewport(x, y, width, height); }
		
		// inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

		inline static void Clear() { s_RendererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}


