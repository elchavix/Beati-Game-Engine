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
		OrthographicCameraController m_CameraController;

		Ref<Scene> m_ActiveScene;
		Entity m_CameraEntity;
		Entity m_SquareEntity;
		Entity m_CircleEntity;
		Entity m_LineEntity;

		float m_FPS = 0.0f;

		bool m_KeyESC = false;
		bool m_MenuVar = false;
	};
}
