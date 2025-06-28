#include "bepch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Beati {

	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

	/*void RendererCommand::BeginScene()
	{
		RendererAPI::BeginScene();
	}

	void RendererCommand::EndScene()
	{
		RendererAPI::EndScene();
	}

	void RendererCommand::Init()
	{
		RendererAPI::Init();
	}

	void RendererCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		RendererAPI::DrawIndexed(vertexArray, indexCount);
	}*/

}
