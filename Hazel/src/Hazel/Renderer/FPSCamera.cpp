#include "hzpch.h"
#include "FPSCamera.h"

namespace Hazel
{
	FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		Zoom(ZOOM),
		m_Rotation(0.0f),
		m_Near(0.1f),
		m_Far(100.0f)
	{
		this->m_Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->RecalculateVectors();
		this->m_ViewMatrix = GetViewMatrix();
	}

	FPSCamera::FPSCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		Zoom(ZOOM),
		m_Rotation(0.0f),
		m_Near(0.1f),
		m_Far(100.0f)
	{
		this->m_Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->RecalculateVectors();
		this->m_ViewMatrix = GetViewMatrix();
	}


	void FPSCamera::SetProjectionMatrix(const float aspect, const float znear, const float zfar)
	{
		this->m_Far = zfar;
		this->m_Near = znear;
		this->m_ProjectionMatrix = glm::perspective(glm::radians(this->Zoom), aspect, znear, zfar);
	}

	void FPSCamera::RecalculateVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // 向量进行归一化，因为当你看向上或下时，他们的长度越来越接近0，导致移动速度变慢。
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));

	}
}