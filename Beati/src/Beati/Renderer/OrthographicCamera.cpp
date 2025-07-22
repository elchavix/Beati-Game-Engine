#include "bepch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>



namespace Beati {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		UpdateViewMatrix();
	}

	void OrthographicCamera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::rotate(glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), m_Position),
			glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
