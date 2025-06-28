#include "bepch.h"
#include "Renderer.h"


namespace Beati {
	
	void Renderer::BeginScene()
	{
		// RendererCommand::BeginScene();
	}

	void Renderer::EndScene()
	{
		// RendererCommand::EndScene();
	}

	//void Renderer::Init()
	//{
	//	// RendererCommand::Init();
	//}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		RendererCommand::DrawIndexed(vertexArray);
	}
}
