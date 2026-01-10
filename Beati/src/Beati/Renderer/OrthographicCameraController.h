#pragma once

#include "Beati/Renderer/OrthographicCamera.h"
#include "Beati/Core/Timestep.h"

#include "Beati/Events/ApplicationEvent.h"
#include "Beati/Events/MouseEvent.h"

namespace Beati {

	class OrthographicCameraBounds
	{
		public:
		float Left, Right;
		float Bottom, Top;

		float GetWidth() const { return Right - Left; }
		float GetHeight() const { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep delta);

		void OnEvent(Event& e);

		void OnResize(float width, float height);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		inline glm::vec3 GetCameraPosition() const { return m_CameraPosition; }
		inline glm::mat4 GetCameraViewMatrix() const { return m_Camera.GetViewMatrix(); }

		inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; CalculateView(); }

		inline const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void CalculateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		bool m_Rotation = false;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f }; // Camera position in world space
		float m_CameraRotation = 0.0f; // Current camera rotation

		float m_CameraTranslationSpeed = 1.4f; // Speed of camera translation
		float m_CameraRotationSpeed = 20.0f; // Speed of camera rotation

		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
	};

}
