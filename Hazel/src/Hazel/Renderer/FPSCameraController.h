#pragma once

#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "FPSCamera.h"

namespace Hazel
{
	class FPSCameraController
	{
	public:
		FPSCameraController(const float aspect);

		FPSCamera& GetCamera() { return this->m_Camera; }
		const FPSCamera& GetCamera() const { return this->m_Camera; }
		

		void OnResize(float width, float height);
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		FPSCamera m_Camera;

		glm::vec3 m_CameraPosition;

		float m_Aspect;

		float m_ZoomLevel;
		float m_CameraTranslationSpeed;
	};
}