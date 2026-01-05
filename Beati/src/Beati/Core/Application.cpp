#include "bepch.h"
#include "Application.h"

#include "Beati/Renderer/Renderer.h"

#include "Input.h"

#include "GLFW/glfw3.h"


namespace Beati {

	Application* Application::s_Instance = nullptr;

	Application::Application() // Initialize the camera with orthographic projection parameters
	{
		BE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		// Constructor implementation
		m_Window = Window::Create();
		m_Window->SetEventCallback(BE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		BE_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		BE_PROFILE_FUNCTION();
		// Push a layer onto the stack
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		BE_PROFILE_FUNCTION();
		// Push an overlay onto the stack
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		BE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		BE_PROFILE_FUNCTION();
		// Run the application logic
		while (m_Running)
		{
			BE_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime(); // TODO: Llavarlo a Platform/
			Timestep deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;
			if (!m_Minimized)
			{
				{
					BE_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(deltaTime);
					}
				}

				m_ImGuiLayer->Begin();
				{
					BE_PROFILE_SCOPE("LayerStack OnImGuiUpdate");
					for (Layer* layer : m_LayerStack)
					{
						layer->OnImGuiRender();
					}
				}
				m_ImGuiLayer->End();
			}
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		return true; 
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		BE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false; // Allow further processing of the event
	}
}
