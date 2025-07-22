#include "bepch.h"
#include "OrthographicCameraController.h"

#include "Beati/Core/Input.h"
#include "Beati/Core/KeyCodes.h"

namespace Beati {


	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-aspectRatio, aspectRatio, -1.0f, 1.0f), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep delta)
	{
		if (Input::IsKeyPressed(BE_KEY_W))
			m_CameraPosition.y -= m_CameraTranslationSpeed * delta;
		else if (Input::IsKeyPressed(BE_KEY_S))
			m_CameraPosition.y += m_CameraTranslationSpeed * delta;
		if (Input::IsKeyPressed(BE_KEY_D))
			m_CameraPosition.x -= m_CameraTranslationSpeed * delta;
		else if (Input::IsKeyPressed(BE_KEY_A))
			m_CameraPosition.x += m_CameraTranslationSpeed * delta;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(BE_KEY_E))
				m_CameraRotation += m_CameraRotationSpeed * delta;
			else if (Input::IsKeyPressed(BE_KEY_Q))
				m_CameraRotation -= m_CameraRotationSpeed * delta;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f); // Prevent zooming out too far
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		// Update camera translation speed based on zoom level
		m_CameraTranslationSpeed = m_ZoomLevel * 1.3f;
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
