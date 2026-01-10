#include "MondongoLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace Beati {

	MondongoLayer::MondongoLayer()
		: Layer("MondongoLayer")
	{
	}

	void MondongoLayer::OnAttach()
	{
		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		m_SquareEntity.GetComponent<TransformComponent>().Scale = { 1.5f, 1.5f, 1.0f };
		m_SquareEntity.GetComponent<TransformComponent>().Translation = { -5.0f, 1.0f, 0.0f };

		m_CircleEntity = m_ActiveScene->CreateEntity("circle");
		m_CircleEntity.AddComponent<CircleRendererComponent>(glm::vec4{ 1.0f, 0.3f, 0.8f, 1.0f });
		m_CircleEntity.GetComponent<TransformComponent>().Scale = { 1.5f, 6.5f, 1.0f };
		m_CircleEntity.GetComponent<TransformComponent>().Translation = { 5.0f, -1.0f, 0.0f };

		m_LineEntity = m_ActiveScene->CreateEntity("line");
		m_LineEntity.AddComponent<LineRendererComponent>(glm::vec4{ 1.0f, 0.3f, 0.8f, 1.0f });
		m_LineEntity.GetComponent<LineRendererComponent>().p0 = glm::vec3{ 0.0f, 0.0f, 0.0f };
		m_LineEntity.GetComponent<LineRendererComponent>().p1 = glm::vec3{ -12.0f, -6.0f, 0.0f };

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		auto& cameraComponent = m_CameraEntity.AddComponent<CameraComponent>();
		cameraComponent.Camera.SetOrthographic(32.0f, -1.0f, 1.0f); // 32 = 16 - (-16)
		cameraComponent.Camera.SetViewportSize(1280, 720); // Ajusta al tamaño de la ventana
	}

	void MondongoLayer::OnDetach()
	{
	}

	void MondongoLayer::OnUpdate(Timestep delta)
	{
		// ---- Update ----
		if (Input::IsKeyPressed(Key::Escape))
		{
			if (!m_KeyESC)
			{
				m_MenuVar = !m_MenuVar;
				m_KeyESC = true;
			}
		}
		else 
		{
			m_KeyESC = false;
		}

		Renderer2D::ResetStats();
		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.4f, 1.0f });
		RenderCommand::Clear();
		m_FPS = 1.0f / delta.GetSeconds(); // TODO: Calculate average FPS

		// ---- Render Scene ----
		m_ActiveScene->OnUpdate(delta);
	}

	void MondongoLayer::OnImGuiRender()
	{
		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("FPS: %d", (int)m_FPS);

		auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

		ImGui::Spacing();
		ImGui::Text("Mouse window position");
		ImGui::Text("X:%d	Y:%d", (int)Input::GetMouseX(), (int)Input::GetMouseY());

		ImGui::Spacing();
		if (m_MenuVar)
		{
			ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.4f, 1.0f), "ESC Menu state: %d", m_MenuVar);
			if (ImGui::Button("Exit", {100, 40}))
				Application::Get().Close();
		}
		else
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.3f, 1.0f), "ESC Menu state: %d", m_MenuVar);
		ImGui::End();
	}

	void MondongoLayer::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& event = (WindowResizeEvent&)e;
			m_ActiveScene->OnViewportResize(event.GetWidth(), event.GetHeight());
		}
	}

}
