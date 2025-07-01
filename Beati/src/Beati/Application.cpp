#include "bepch.h"
#include "Application.h"

#include "Beati/Log.h"

#include "Beati/Renderer/Renderer.h"

#include "Input.h"

#include "GLFW/glfw3.h"


namespace Beati {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() // Initialize the camera with orthographic projection parameters
	{
		BE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		// Constructor implementation
		m_Window = Ref<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
			float time = (float)glfwGetTime(); // TODO: Llavarlo a Platform/
			Timestep deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(deltaTime);
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
