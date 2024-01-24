#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Core/TimeStep.h"
namespace Hazel
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return this->m_Camera; }
		const OrthographicCamera& GetCamera() const { return this->m_Camera; }

		void OnResize(float width, float height);

		void OnUpdate(TimeStep& ts);
		void OnEvent(Event& e);

		float GetZoomLevel() { return this->m_ZoomLevel; }
		void SetZoomLevel(float zoomLevel) { this->m_ZoomLevel = zoomLevel; }

	private:
		bool MouseEventScrolled(MouseScrolledEvent& e);
		bool WindowResized(WindowResizeEvent& e);
	private:
		//重点：定义的顺序决定初始化的顺序！！！
		float m_ZoomLevel;
		float m_AspectRatio;

		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPos;

		float m_CameraRotation;
		float m_CameraTranslationSpeed;
		float m_CameraRotationSpeed;


		bool m_Rotation;
	};
}