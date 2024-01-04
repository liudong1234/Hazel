#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_Position(0.0f, 0.0f, 0.0f),
		m_ViewMatrix(1.0f)
	{
		this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(this->m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		this->m_ViewMatrix = glm::inverse(transform);//求逆
		this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
	}
}