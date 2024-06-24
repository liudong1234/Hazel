#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	const float YAW = -90.0f;  //ƫ����
	const float PITCH = 0.0f;   //������
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class FPSCamera
	{
	public:

		// ���ѡ��

		// ʹ��������Ϊ�����Ĺ��캯��  
		FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		// ʹ�ñ���ֵ��Ϊ�����Ĺ��캯��  
		FPSCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		// ����ʹ��ŷ���Ǻ�LookAt�������ó�����ͼ����

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateVectors(); }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateVectors(); }
		void SetZoom(const float zoom) { this->Zoom = zoom;  RecalculateVectors(); }
		void SetProjectionMatrix(const float aspect, const float znear, const float zfar);


		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const float GetRotation() const { return m_Rotation; }
		inline const glm::vec3 GetFront() const { return this->Front; }
		inline const glm::vec3 GetUp() const { return this->Up; }
		inline const glm::vec3 GetRight() const { return this->Right; }
		inline const glm::vec3 GetWorldUp() const { return this->WorldUp; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + Front, Up); }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateVectors();

	private:
		// �������
		glm::vec3 m_Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp; // ��������ϵ���Ϸ�����  

		float Zoom;//����ֵ
		// ŷ����
		float Yaw;
		float Pitch;

		float m_Rotation;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Near;
		float m_Far;
	};

}