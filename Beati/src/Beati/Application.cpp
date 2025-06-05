#include "bepch.h"
#include "Application.h"
#include "Beati/Events/ApplicationEvent.h"
#include "Beati/Log.h"

#include "GLFW/glfw3.h"

namespace Beati {
	Application::Application()
	{
		// Constructor implementation
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		// Destructor implementation
	}

	void Application::Run()
	{
		// Run the application logic
		while (true)
		{
			glClearColor(0.1f, 1.0f, 0.1f, 0.7f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}
