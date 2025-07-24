#include <Beati.h>
// ---- EntryPonit para windows ----
#include "Beati/Core/EntryPoint.h"
// ---------------------------------
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class EjemploCapa : public Beati::Layer
{
public:
	EjemploCapa() : Layer("EjemploCapa"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray = Beati::VertexArray::Create();
		 
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1 (X, Y, Z, R, G, B, A)
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Vertex 2 (X, Y, Z, R, G, B, A)
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f  // Vertex 3 (X, Y, Z, R, G, B, A)
		};
		Beati::Ref<Beati::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Beati::VertexBuffer::Create(vertices, sizeof(vertices)));


		Beati::BufferLayout layout = {
			{ Beati::ShaderDataType::Float3, "a_Position"},
			{ Beati::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };

		Beati::Ref<Beati::IndexBuffer> indexBuffer;
		indexBuffer.reset(Beati::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Beati::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// Bottom Left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// Bottom Right
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,	// Top Right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f	// Top Left
		};

		Beati::Ref<Beati::VertexBuffer> squareVB;
		squareVB.reset(Beati::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Beati::ShaderDataType::Float3, "a_Position" },
			{ Beati::ShaderDataType::Float2, "a_TextCoord" }
			});
		
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, // First Triangle
			2, 3, 0  // Second Triangle
		};
		Beati::Ref<Beati::IndexBuffer> squareIB;
		squareIB.reset(Beati::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexShaderSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentShaderSource = R"(
			#version 330 core
			out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = (Beati::Shader::Create("CubosShader", vertexShaderSource, fragmentShaderSource));

		std::string vertexShaderSrc2 = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentShaderSrc2 = R"(
			#version 330 core
			out vec4 color;
			in vec3 v_Position;
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_Shader2 = (Beati::Shader::Create("CubosShader", vertexShaderSrc2, fragmentShaderSrc2));

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = (Beati::Texture2D::Create("assets/textures/test2.png"));

		std::dynamic_pointer_cast<Beati::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Beati::OpenGLShader>(textureShader)->UploadloadUniformInt("u_Texture", 0);
	}

	void OnAttach() override
	{

	}

	void OnDetach() override
	{

	}

	void OnUpdate(Beati::Timestep delta) override
	{
		// ---- Update ----
		m_CameraController.OnUpdate(delta);

		// ---- Render ----
		Beati::RendererCommand::SetClearColor({ 0.2f, 0.4f, 0.5f, 1.0f });
		Beati::RendererCommand::Clear();

		Beati::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Beati::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Beati::OpenGLShader>(m_Shader2)->UploadloadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 50; y++)
		{
			for (int x = 0; x < 50; x++)
			{
				glm::vec3 position(x * 0.11f, y * 0.11f, 1.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				Beati::Renderer::Submit(m_Shader2, m_SquareVA, transform);
			}

		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();

		Beati::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Tiangle
		// Beati::Renderer::Submit(m_Shader, m_VertexArray);

		Beati::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Beati::Event& event) override
	{
		m_CameraController.OnEvent(event);

		if (event.GetEventType() == Beati::EventType::WindowResize)
		{
			// Beati::WindowResizeEvent& re = (Beati::WindowResizeEvent&)event;
			// float zoomLvl = m_CameraController.GetZoomLevel();
		}

	}

private:

	Beati::ShaderLibrary m_ShaderLibrary;
	Beati::Ref<Beati::Shader> m_Shader;
	Beati::Ref<Beati::VertexArray> m_VertexArray;

	Beati::Ref<Beati::Shader> m_Shader2;
	Beati::Ref<Beati::VertexArray> m_SquareVA;

	Beati::Ref<Beati::Texture2D> m_Texture;

	Beati::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class Sandbox : public Beati::Application
{
public:
	Sandbox()
	{
		// PushLayer(new EjemploCapa());
		PushLayer(new Sandbox2D());
	}

    ~Sandbox() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Sandbox();
}

