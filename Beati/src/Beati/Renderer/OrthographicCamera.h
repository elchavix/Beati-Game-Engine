#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Beati {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top)
			: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMatrix(1.0f)
		{
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }

		inline void SetRotation(float rotation) { m_Rotation = rotation; UpdateViewMatrix(); }

		inline const glm::vec3& GetPosition() const { return m_Position; }

		inline float GetRotation() const { return m_Rotation; }

		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		inline void UpdateViewMatrix() { m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)); m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}