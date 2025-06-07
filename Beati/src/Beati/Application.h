#pragma once

#include "Core.h"

#include "Window.h"
#include "Beati/LayerStack.h"
#include "Beati/Events/Event.h"
#include "Beati/Events/ApplicationEvent.h"



namespace Beati {

	class BEATI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(Event& e);

		std::unique_ptr<class Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in the client
	Application* CreateApplication();
}

