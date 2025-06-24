#pragma once

#include "Core.h"

#include "Window.h"
#include "Beati/LayerStack.h"
#include "Beati/Events/Event.h"
#include "Beati/Events/ApplicationEvent.h"

#include "Beati/ImGui/ImGuiLayer.h"

#include "Beati/Renderer/Shader.h"
#include "Beati/Renderer/Buffer.h"



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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(Event& e);

		std::unique_ptr<class Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}

