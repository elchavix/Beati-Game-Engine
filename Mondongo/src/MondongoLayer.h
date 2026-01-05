#pragma once

#include "Beati.h"

namespace Beati {

	class MondongoLayer : public Layer
	{
	public:
		MondongoLayer();
		virtual ~MondongoLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep delta) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		ShaderLibrary m_ShaderLibrary;
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Texture2D> m_Texture;

		OrthographicCameraController m_CameraController;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec4 m_TextureColor = glm::vec4(1.0f);
		float m_FPS = 0.0f;
		float m_TextureRotation = 0.0f;
		float m_TextureTiling = 1.0f;
		bool m_RenderQuad = true;

		bool m_KeyESC = false;
		bool m_MenuVar = false;
	};
}
