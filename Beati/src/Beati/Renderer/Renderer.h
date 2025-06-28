#pragma once
#include "RendererCommand.h"


namespace Beati {
	
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		// static void Init();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}