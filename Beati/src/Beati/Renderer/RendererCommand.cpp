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


	void RendererCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		RendererAPI::DrawIndexed(vertexArray, indexCount);
	}*/

}
