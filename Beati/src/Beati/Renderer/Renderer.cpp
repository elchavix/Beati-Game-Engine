#include "bepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Beati {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;


	void Renderer::Init()
	{
		RendererCommand::Init();
	}

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

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind(); 
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadloadUniformMatrix4f("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadloadUniformMatrix4f("u_Transform", transform);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}
