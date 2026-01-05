#pragma once

#include "Beati/Core/Base.h"

#include "Beati/Core/Window.h"
#include "Beati/Core/LayerStack.h"
#include "Beati/Events/Event.h"
#include "Beati/Events/ApplicationEvent.h"

#include "Beati/ImGui/ImGuiLayer.h"

#include "Beati/Core/Timestep.h"

int main(int argc, char** argv);


namespace Beati {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(Event& e);
		
		bool OnWindowResize(WindowResizeEvent& e);


		Ref<class Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}

