#include "MondongoLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Beati {

	MondongoLayer::MondongoLayer()
		: Layer("MondongoLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void MondongoLayer::OnAttach()
	{
		BE_PROFILE_FUNCTION();

		m_Texture = (Texture2D::Create("assets/textures/cubo.png"));

		m_CameraController.SetZoomLevel(5.0f);
	}

	void MondongoLayer::OnDetach()
	{
		// BE_PROFILE_FUNCTION();
		// TODO: UI Sistem?
	}

	void MondongoLayer::OnUpdate(Timestep delta)
	{
		// ---- Update ----
		m_CameraController.OnUpdate(delta);

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

		// ---- Render ----
		Renderer2D::ResetStats();
		{
			BE_PROFILE_SCOPE("Render Prep");
			RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.4f, 1.0f });
			RenderCommand::Clear();
			m_FPS = 1.0f / delta.GetSeconds(); // TODO: Calculate average FPS
		}

		{
			BE_PROFILE_SCOPE("Render Draw");
			Renderer2D::BeginScene(m_CameraController.GetCamera());

			if (m_RenderQuad)
				Renderer2D::DrawRotatedQuad({ 0.5f, 0.2f, -0.1f }, { 5.0f, 5.0f }, m_TextureRotation, m_Texture, m_TextureTiling, m_TextureColor);
			for (int y = -5; y < 5; y++)
			{
				for (int x = -5; x < 5; x++)
				{
					glm::vec4 color = { (float)(x + 5) / 10.0f, 0.4f, (float)(y + 5) / 10.0f, 0.8f };
					if ((x + y) % 2 == 0)
						Renderer2D::DrawQuad({ (float)x, (float)y, 0.0f }, { 0.45f, 0.45f }, color);
					else
						Renderer2D::DrawQuad({ (float)x, (float)y, 0.0f }, { 0.45f, 0.45f }, m_Texture, 1.0f, color);
				}
			}

			Renderer2D::DrawQuad({ 1.0f, -0.5f, 0.1f }, { 0.5f, 0.75f }, { m_SquareColor });

			Renderer2D::EndScene();
		}
	}

	void MondongoLayer::OnImGuiRender()
	{
		BE_PROFILE_FUNCTION();

		ImGui::Begin("Settings");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("FPS: %d", (int)m_FPS);

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::ColorEdit4("Texture Color", glm::value_ptr(m_TextureColor));
		ImGui::SliderFloat("Texture Rotation", &m_TextureRotation, 0.0f, 6.283185307179586476925286766559f);
		ImGui::SliderFloat("Texture Tiling", &m_TextureTiling, 1.0f, 60.0f);
		ImGui::Checkbox("Texture Rotation Quad", &m_RenderQuad);

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
		BE_CORE_INFO("{0}", e.ToString());
		m_CameraController.OnEvent(e);
	}

}
