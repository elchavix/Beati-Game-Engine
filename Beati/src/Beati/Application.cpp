#include "bepch.h"
#include "Application.h"

#include "Beati/Log.h"

#include "GLFW/glfw3.h"

namespace Beati {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		// Constructor implementation
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
		// Destructor implementation
	}

	void Application::OnEvent(Event& e)
	{
		// Handle events
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		BE_CORE_TRACE("Event: {0}", e.ToString());
	}

	void Application::Run()
	{
		// Run the application logic
		while (m_Running)
		{
			glClearColor(0.1f, 1.0f, 0.1f, 0.7f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(Event& e)
	{
		m_Running = false;
		return true; 
	}
}
