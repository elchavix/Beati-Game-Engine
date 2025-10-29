#include "bepch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"

#include "Texture.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Beati {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture> WhiteTexture;
	};

	static Renderer2DStorage* s_Data = new Renderer2DStorage();

	void Renderer2D::Init()
	{
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Top Right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // Top Left
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TextCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, // First Triangle
			2, 3, 0  // Second Triangle
		};
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1); // Create a 1x1 white texture
		s_Data->WhiteTexture->SetData(new uint8_t[4]{ 255, 255, 255, 255 }, 4); // Fill it with white color

		s_Data->TextureShader = (Shader::Create("assets/shaders/Texture.glsl"));
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0); // Set the texture slot to 0
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * // TODO: Add rotation here
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
	{
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * // TODO: Add rotation here
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RendererCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawString(const std::string& text, const glm::vec3& position, float size, const glm::vec4& color)
	{

	}

	RendererStats Renderer2D::GetStats()
	{
		return RendererStats();
	}
}

