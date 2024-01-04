#include "hzpch.h"

#include "OrthoGraphicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top):
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_Rotation(0.0f),
		m_Position(0.0f, 0.0f, 0.0f)
	{
		this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
	}

	void OrthoGraphicCamera::ReCaculateViewProjection()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->m_Position) *
			glm::rotate(glm::mat4(1.0f), this->m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		this->m_ViewMatrix = glm::inverse(transform);
		this->m_ViewProjectionMatrix = this->m_ProjectionMatrix * this->m_ViewMatrix;
	}
}