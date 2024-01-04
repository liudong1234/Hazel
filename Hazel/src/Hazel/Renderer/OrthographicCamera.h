#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		glm::vec3 GetPosition() const { return this->m_Position; }
		void SetPosition(const glm::vec3& position)
		{
			this->m_Position = position;
			this->RecalculateViewMatrix();
		}
		float GetRotation() const { return this->m_Rotation; }
		void SetRotation(const float rotation)
		{
			this->m_Rotation = rotation;
			this->RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return this->m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return this->m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return this->m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	};
}