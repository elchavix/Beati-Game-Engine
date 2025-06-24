#include "bepch.h"
#include "Application.h"

#include "Beati/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Beati {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		// Constructor implementation
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f, // Vertex 1 (X, Y, Z)
			 0.5f, -0.5f, 0.0f, // Vertex 2 (X, Y, Z)
			 0.0f,  0.5f, 0.0f  // Vertex 3 (X, Y, Z)
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		unsigned int indices[3] = { 0, 1, 2 };
		
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexShaderSource = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position; // Pass position to fragment shader
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentShaderSource = R"(
			#version 330 core
			out vec4 color;
			in vec3 v_Position; // Receive position from vertex shader
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0); // Orange color
			}
		)";
		
		m_Shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));
	}

	Application::~Application()
	{
		// Destructor implementation
	}

	void Application::PushLayer(Layer* layer)
	{
		// Push a layer onto the stack
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		// Push an overlay onto the stack
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		// Handle events
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		// Run the application logic
		while (m_Running)
		{
			glClearColor(0.2f, 0.4f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);


			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		return true; 
	}
}
