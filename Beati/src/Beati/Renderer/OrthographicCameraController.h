#pragma once

#include "Beati/Renderer/OrthographicCamera.h"
#include "Beati/Core/Timestep.h"

#include "Beati/Events/ApplicationEvent.h"
#include "Beati/Events/MouseEvent.h"

namespace Beati {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep delta);

		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		bool m_Rotation = false;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f }; // Camera position in world space
		float m_CameraRotation = 0.0f; // Current camera rotation

		float m_CameraTranslationSpeed = 1.0f; // Speed of camera translation
		float m_CameraRotationSpeed = 20.0f; // Speed of camera rotation

		OrthographicCamera m_Camera;
	};

}
