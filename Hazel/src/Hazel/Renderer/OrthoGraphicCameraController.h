#pragma once

#include "Hazel/Renderer/OrthoGraphicCamera.h"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
namespace Hazel
{
	class OrthoGraphicCameraController
	{
	public:
		OrthoGraphicCameraController(float aspectRatio, bool rotation = false);

		OrthoGraphicCamera& GetCamera() { return this->m_Camera; }
		const OrthoGraphicCamera& GetCamera() const { return this->m_Camera; }

		

		void OnResize(float width, float height);
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel;

		OrthoGraphicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraRotation;
		bool m_Rotation;

		float m_CameraRotationSpeed;
		float m_CameraTranslationSpeed;
	};

}