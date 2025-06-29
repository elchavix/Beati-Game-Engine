#include <Beati.h>

class EjemploCapa : public Beati::Layer
{
public:
	EjemploCapa() : Layer("EjemploCapa"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_VertexArray.reset(Beati::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Vertex 1 (X, Y, Z, R, G, B, A)
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Vertex 2 (X, Y, Z, R, G, B, A)
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f  // Vertex 3 (X, Y, Z, R, G, B, A)
		};
		std::shared_ptr<Beati::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Beati::VertexBuffer::Create(vertices, sizeof(vertices)));


		Beati::BufferLayout layout = {
			{ Beati::ShaderDataType::Float3, "a_Position"},
			{ Beati::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };

		std::shared_ptr<Beati::IndexBuffer> indexBuffer;
		indexBuffer.reset(Beati::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Beati::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f, // Bottom Left
			 0.75f, -0.75f, 0.0f, // Bottom Right
			 0.75f,  0.75f, 0.0f, // Top Right
			-0.75f,  0.75f, 0.0f  // Top Left
		};

		std::shared_ptr<Beati::VertexBuffer> squareVB;
		squareVB.reset(Beati::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Beati::ShaderDataType::Float3, "a_Position"}
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, // First Triangle
			2, 3, 0  // Second Triangle
		};
		std::shared_ptr<Beati::IndexBuffer> squareIB;
		squareIB.reset(Beati::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexShaderSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjectionMatrix;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Beati::Shader(vertexShaderSource, fragmentShaderSource));

		std::string vertexShaderSrc2 = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjectionMatrix;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentShaderSrc2 = R"(
			#version 330 core
			out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader2.reset(new Beati::Shader(vertexShaderSrc2, fragmentShaderSrc2));

	}

	void OnAttach() override
	{

	}

	void OnDetach() override
	{

	}

	void OnUpdate(Beati::Timestep delta) override
	{
		if (Beati::Input::IsKeyPressed(BE_KEY_W))
			m_Camera.SetPosition(m_Camera.GetPosition() - glm::vec3(0.0f, m_CameraSpeed * delta, 0.0f));
		else if (Beati::Input::IsKeyPressed(BE_KEY_S))
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.0f, m_CameraSpeed * delta, 0.0f));
		if (Beati::Input::IsKeyPressed(BE_KEY_D))
			m_Camera.SetPosition(m_Camera.GetPosition() - glm::vec3(m_CameraSpeed * delta, 0.0f, 0.0f));
		else if (Beati::Input::IsKeyPressed(BE_KEY_A))
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(m_CameraSpeed * delta, 0.0f, 0.0f));
		if (Beati::Input::IsKeyPressed(BE_KEY_E))
			m_Camera.SetRotation(m_Camera.GetRotation() + m_CameraRotationSpeed * delta);
		else if (Beati::Input::IsKeyPressed(BE_KEY_Q))
			m_Camera.SetRotation(m_Camera.GetRotation() - m_CameraRotationSpeed * delta);
		if (Beati::Input::IsKeyPressed(BE_KEY_R))
			m_CameraRotationSpeed = 20.0f;
		if (Beati::Input::IsKeyReleased(BE_KEY_R))
			m_CameraRotationSpeed = 10.0f;

		Beati::RendererCommand::SetClearColor({ 0.2f, 0.4f, 0.5f, 1.0f });
		Beati::RendererCommand::Clear();

		Beati::Renderer::BeginScene(m_Camera);

		Beati::Renderer::Submit(m_Shader2, m_SquareVA);

		Beati::Renderer::Submit(m_Shader, m_VertexArray);

		Beati::Renderer::EndScene();
	}

	void OnEvent(Beati::Event& event) override
	{
	}

private:
	std::shared_ptr<Beati::Shader> m_Shader;
	std::shared_ptr<Beati::VertexArray> m_VertexArray;

	std::shared_ptr<Beati::Shader> m_Shader2;
	std::shared_ptr<Beati::VertexArray> m_SquareVA;

	Beati::OrthographicCamera m_Camera;

	float m_CameraSpeed = 0.4f; // Speed of camera movement
	float m_CameraRotationSpeed = 10.0f; // Speed of camera rotation
};


class Sandbox : public Beati::Application
{
public:
	Sandbox()
	{
		PushLayer(new EjemploCapa());
	}

    ~Sandbox() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Sandbox();
}

