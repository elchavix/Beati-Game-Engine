#pragma once

#include "Beati.h"

class SandboxLayer : public Beati::Layer
{
public: 
	SandboxLayer();
	virtual ~SandboxLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Beati::Timestep delta) override;
	void OnImGuiRender() override;
	void OnEvent(Beati::Event& e) override;

private:
	Beati::ShaderLibrary m_ShaderLibrary;
	Beati::Ref<Beati::Shader> m_FlatColorShader;
	Beati::Ref<Beati::VertexArray> m_SquareVA;
	Beati::Ref<Beati::Texture2D> m_Texture;
	Beati::Ref<Beati::Framebuffer> m_Framebuffer;
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	Beati::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_TextureColor = glm::vec4(1.0f);
	float m_TextureRotation = 0.0f;
	float m_TextureTiling = 1.0f;
	bool m_RenderQuad = true;
};
