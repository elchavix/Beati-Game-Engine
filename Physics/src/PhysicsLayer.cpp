#include "PhysicsLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"


PhysicsLayer::PhysicsLayer()
	: Layer("PhysicsLayer")
{}

void PhysicsLayer::OnAttach()
{
	m_ActiveScene = Beati::CreateRef<Beati::Scene>();

	m_BaseballTexture = Beati::Texture2D::Create("assets/textures/baseball_ball.png");

	m_RopeAnchor = m_ActiveScene->CreateEntity("RopeAnchor");
	m_RopeAnchor.GetComponent<Beati::TransformComponent>().Translation = glm::vec3(0.0f, 5.0f, 0.0f);  // Posición inicial

	auto& rope = m_RopeAnchor.AddComponent<Beati::RopeComponent>();
	rope.IsAnchored = true;
	rope.Density = 1.0f;
	rope.NumSegments = 20;
	rope.EndOffset = { 0.0f, -10.0f, 0.0f };
	rope.SegmentWidth = 0.1f;

	m_QuadEntity = m_ActiveScene->CreateEntity("Quad Entity");
	m_QuadEntity.AddComponent<Beati::Rigidbody2DComponent>().Type = Beati::Rigidbody2DComponent::BodyType::Static;
	m_QuadEntity.AddComponent<Beati::BoxCollider2DComponent>().Friction = 5.0f;
	m_QuadEntity.AddComponent<Beati::SpriteRendererComponent>().Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_QuadEntity.GetComponent<Beati::TransformComponent>().Translation = { 2.0f, -5.0f, 0.0f };
	m_QuadEntity.GetComponent<Beati::TransformComponent>().Scale = glm::vec3(10.0f, 1.0f, 0.0f);

	m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
	auto& cameraComponent = m_CameraEntity.AddComponent<Beati::CameraComponent>();
	m_CameraEntity.AddComponent<Beati::InputMovementComponent>();

	cameraComponent.Camera.SetOrthographic(32.0f, -1.0f, 1.0f); // 32 = 16 - (-16)
	cameraComponent.Camera.SetViewportSize(1280, 720); // Ajusta al tamaño de la ventana
	cameraComponent.Camera.SetZoomLevel(5.0f);

	Beati::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	fbSpec.Attachments = { Beati::FramebufferTextureFormat::RGBA8 };
	m_Framebuffer = Beati::Framebuffer::Create(fbSpec);

	m_ActiveScene->OnPhysics2DStart();
}

void PhysicsLayer::OnDetach()
{
	m_ActiveScene->OnPhysics2DStop();
}

void PhysicsLayer::OnUpdate(Beati::Timestep delta)
{
	// ---- Update ----
	m_MousePos = CalculateMousePosInViewport(); // TODO: Optimizar para no llamarlo siempre

	m_CameraEntity.GetComponent<Beati::CameraComponent>().IsFocused = m_ViewportFocused; // Permitir mover la cámara solo si el viewport está enfocado

	// Elimina circulos
	for (int i = 0; i < m_Circulos.size(); i++)
	{
		auto& circulo = m_Circulos[i];

		auto& rb2d = circulo.GetComponent<Beati::Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		if (body)
		{
			circulo.GetComponent<Beati::ParticleEmitterComponent>().EmissionRate = 8.0f * (glm::abs(body->GetLinearVelocity().x) + glm::abs(body->GetLinearVelocity().y));
		}

		if (circulo.GetComponent<Beati::TransformComponent>().Translation.y < -10.0f)
		{
			m_ActiveScene->DestroyEntity(circulo);
			m_Circulos.erase(std::remove(m_Circulos.begin(), m_Circulos.end(), circulo), m_Circulos.end());
		}
	}

	// ---- Render ----
	Beati::Renderer2D::ResetStats();
	m_Framebuffer->Bind();
	Beati::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.4f, 1.0f });
	Beati::RenderCommand::Clear();

	// ---- Render Scene ----
	m_ActiveScene->OnUpdate(delta);

	m_Framebuffer->Unbind();
}

void PhysicsLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();
	
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) Beati::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	ImGui::SliderFloat("Circle Size", &m_CircleScale, 0.1f, 10.0f);
	ImGui::End();

	ImGui::Begin("Draw Stats");
	auto stats = Beati::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Circles: %d", m_Circulos.size());
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("Cam Rotation: %.2f", m_CameraEntity.GetComponent<Beati::TransformComponent>().Rotation.z);
	ImGui::End();
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	Beati::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	ImVec2 viewportPos = ImGui::GetCursorScreenPos();
	m_ViewportPos = { viewportPos.x, viewportPos.y };

	if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0 &&
		(m_ViewportSize.x != viewportSize.x || m_ViewportSize.y != viewportSize.y))
	{
		m_ViewportSize = { viewportSize.x, viewportSize.y };
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	if (m_ViewportSize.x == 0 || m_ViewportSize.y == 0)
	{
		m_ViewportSize = { viewportSize.x, viewportSize.y };
	}

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void PhysicsLayer::OnEvent(Beati::Event& e)
{
	m_ActiveScene->OnEvent(e);

	if (e.GetEventType() == Beati::EventType::MouseButtonPressed && m_ViewportHovered)
	{
		Beati::Entity circulo = m_ActiveScene->CreateEntity("circulo G");
		circulo.AddComponent<Beati::SpriteRendererComponent>().Texture = m_BaseballTexture;
		circulo.GetComponent<Beati::TransformComponent>().Scale = glm::vec3(m_CircleScale);
		circulo.GetComponent<Beati::TransformComponent>().Translation = glm::vec3(m_MousePos.WorldPos.x, m_MousePos.WorldPos.y, 0.0f);
		circulo.AddComponent<Beati::Rigidbody2DComponent>(Beati::Rigidbody2DComponent::BodyType::Dynamic);
		circulo.AddComponent<Beati::CircleCollider2DComponent>().Density = 5.5f;
		
		auto& pec = circulo.AddComponent<Beati::ParticleEmitterComponent>();
		pec.EmissionRate = 50.0f;					// 50 partículas/seg
		pec.MaxParticles = 1000;
		pec.ParticleLifetime = 3.0f;
		pec.InitialVelocityMin = { -2.0f, 1.0f, 0.0f };
		pec.InitialVelocityMax = { 2.0f, 3.0f, 0.0f };
		pec.Color = { 1.0f, 0.5f, 0.0f, 1.0f };		// Naranja
		pec.GravityScale = 0.5f;					// Gravedad ligera
		pec.IsEmitting = true;


		auto& rb2d = circulo.GetComponent<Beati::Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		if (body)
		{
			body->SetLinearVelocity(b2Vec2(m_MousePos.MouseDelta.x * 15.0f, m_MousePos.MouseDelta.y * 15.0f));
		}

		m_Circulos.push_back(circulo);
	}
}

MousePos PhysicsLayer::CalculateMousePosInViewport()
{
	MousePos input;

	ImVec2 screenMousePos = ImGui::GetMousePos();
	input.ScreenPos = { screenMousePos.x, screenMousePos.y };

	input.ViewportLocalPos.x = input.ScreenPos.x - m_ViewportPos.x;
	input.ViewportLocalPos.y = input.ScreenPos.y - m_ViewportPos.y;

	input.NormalizedPos.x = (input.ViewportLocalPos.x / m_ViewportSize.x - 0.5f) * 2.0f * m_CameraEntity.GetComponent<Beati::CameraComponent>().Camera.GetWidth() * 0.5f;
	input.NormalizedPos.y = (0.5f - input.ViewportLocalPos.y / m_ViewportSize.y) * 2.0f * m_CameraEntity.GetComponent<Beati::CameraComponent>().Camera.GetHeight() * 0.5f;

	glm::vec3 cameraPos = m_CameraEntity.GetComponent<Beati::TransformComponent>().Translation;
	float cameraRotationDegrees = m_CameraEntity.GetComponent<Beati::TransformComponent>().Rotation.z; // 
	float cameraRotationRadians = glm::radians(cameraRotationDegrees); // Convertir a radianes

	glm::vec3 worldPos = { input.NormalizedPos.x + cameraPos.x, input.NormalizedPos.y + cameraPos.y, 0.0f };

	// Aplicar rotación inversa usando matriz GLM
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), cameraRotationRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 relativePos = worldPos - cameraPos;
	glm::vec3 rotatedPos = glm::vec3(rotationMatrix * glm::vec4(relativePos, 1.0f));

	input.WorldPos.x = rotatedPos.x + cameraPos.x;
	input.WorldPos.y = rotatedPos.y + cameraPos.y;

	input.MouseDelta = input.WorldPos - m_MousePos.WorldPos;

	return input;
}
