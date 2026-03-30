#pragma once

#include "Beati.h"

struct MousePos
{
	glm::vec2 ScreenPos = { 0, 0 };
	glm::vec2 ViewportLocalPos = { 0, 0 };
	glm::vec2 NormalizedPos = { 0, 0 };
	glm::vec2 WorldPos = { 0, 0 };
	glm::vec2 MouseDelta = { 0, 0 }; // TODO: Refinar

	MousePos() = default;
};

class PhysicsLayer : public Beati::Layer
{
public: 
	PhysicsLayer();
	virtual ~PhysicsLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Beati::Timestep delta) override;
	void OnImGuiRender() override;
	void OnEvent(Beati::Event& e) override;

private:
	MousePos CalculateMousePosInViewport(); // TODO: Hacer que funcione con SceneCameraController.

private:
	Beati::Ref<Beati::Scene> m_ActiveScene;

	Beati::Entity m_CameraEntity;

	Beati::Entity m_QuadEntity;
	std::vector<Beati::Entity> m_Circulos;
	Beati::Entity m_RopeAnchor;


	Beati::Ref<Beati::Texture2D> m_BaseballTexture;
	float m_CircleScale = 0.3f;

	Beati::Ref<Beati::Framebuffer> m_Framebuffer;

	MousePos m_MousePos;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportPos = { 0.0f, 0.0f }; // TODO: Quizá meter estas dos cosas en un struct.

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;
};
