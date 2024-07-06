#pragma once
#include "Camera.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
	class EditorCamera:
		public Camera
	{
	public:
		EditorCamera();
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		~EditorCamera();

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return this->m_Distance; }
		inline float SetDistance(float distance) { this->m_Distance = distance; }

		inline void SetViewportSize(float width, float height)
		{
			this->m_ViewportWidth = width;
			this->m_ViewportHeight = height;
			this->UpdateProjection();
		}

		const glm::mat4& GetViewMatrix() const { return this->m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return this->m_Projection * this->m_ViewMatrix;}

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return this->m_Position; }
		glm::quat GetOrientation() const;

		float GetPicth() const { return this->m_Pitch; }
		float GetYaw() const { return this->m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();
		bool OnMouseScroll(MouseScrolledEvent& e);


		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculaePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_Fov, m_AspectRatio, m_NearClip, m_FarClip;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position;
		glm::vec3 m_FocalPoint;

		glm::vec2 m_InitialMousePositon;

		float m_Distance;
		float m_Pitch;
		float m_Yaw;
		float m_ViewportWidth, m_ViewportHeight;
	};
}
