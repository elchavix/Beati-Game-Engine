#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = (Beati::Texture2D::Create("assets/textures/cubo.png"));
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Beati::Timestep delta)
{
	// ---- Update ----
	m_CameraController.OnUpdate(delta);

	// ---- Render ----
	Beati::RendererCommand::SetClearColor({ 0.2f, 0.4f, 0.5f, 1.0f });
	Beati::RendererCommand::Clear();

	Beati::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	Beati::Renderer2D::DrawQuad({ 0.5f, 0.2f, -0.1f}, {5.0f, 5.0f}, m_Texture, 0.0f);
	Beati::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.6f, 0.0f, 1.0f });
	Beati::Renderer2D::DrawQuad({ 1.0f, -0.5f }, { 0.5f, 0.75f }, { m_SquareColor });

	Beati::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Beati::Event& e)
{
	m_CameraController.OnEvent(e);
}
