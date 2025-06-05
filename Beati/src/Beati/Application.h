#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"


namespace Beati {

	class BEATI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<class Window> m_Window;
	};

	// To be defined in the client
	Application* CreateApplication();
}

