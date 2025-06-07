#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Beati/Events/ApplicationEvent.h"

#include "Window.h"


namespace Beati {

	class BEATI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(Event& e);

		std::unique_ptr<class Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in the client
	Application* CreateApplication();
}

