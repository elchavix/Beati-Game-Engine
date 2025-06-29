#include "bepch.h"
#include "Renderer.h"


namespace Beati {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		// RendererCommand::EndScene();
	}

	//void Renderer::Init()
	//{
	//	// RendererCommand::Init();
	//}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind(); 
		shader->UploadloadUniformMatrix4f("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}
