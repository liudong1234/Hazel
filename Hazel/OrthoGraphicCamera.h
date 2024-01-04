#pragma once

#include <glm/glm.hpp>
namespace Hazel
{
	class OrthoGraphicCamera
	{
	public:
		OrthoGraphicCamera(float left, float right,float bottom, float top);
		
		const glm::vec3& GetPosition() const { return this->m_Position; }
		void SetPosition(glm::vec3& position)
		{
			this->m_Position = position;  this->ReCaculateViewProjection();
		}

		const glm::mat4& GetProjection() const { return this->m_ProjectionMatrix; }
		void SetProjection(const glm::mat4 projection) {this->m_ProjectionMatrix = projection;}

		const glm::mat4& GetViewMatrix() const { return this->m_ViewMatrix; }
		void SetViewMatrix(const glm::mat4 view) { this->m_ViewMatrix = view; }

		float GetRotation() const { return this->m_Rotation; }
		void SetRotation(const float rotate) 
		{ 
			this->m_Rotation = rotate; this->ReCaculateViewProjection(); 
		}

	private:
		void ReCaculateViewProjection();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;

		float m_Rotation;
	};
}