#include "Application.h"

#include "Beati/Events/ApplicationEvent.h"
#include "Beati/Log.h"

namespace Beati {
	Application::Application()
	{
		// Constructor implementation
	}

	Application::~Application()
	{
		// Destructor implementation
	}

	void Application::Run()
	{
		WindowResizeEvent resizeEvent(1280, 720);	// Pruebo el evento de redimensionamiento de ventana.
		BE_TRACE("Window resized to: {0}x{1}", resizeEvent.GetWidth(), resizeEvent.GetHeight());
		// Run the application logic
		while (true);
	}
}
