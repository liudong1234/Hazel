#include "hzpch.h"
#include "OrthoGraphicCameraController.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
namespace Hazel
{
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1.0f),
		m_Rotation(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_CameraTranslationSpeed(3.0f), m_CameraRotationSpeed(180.0f),
		m_CameraPosition({ 0.0f, 0.0f, 0.0f }),
		m_CameraRotation(0.0f)
	{
	}

	void OrthoGraphicCameraController::OnResize(float width, float height)
	{
		this->m_AspectRatio = width / height;
		this->m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
	}

	void OrthoGraphicCameraController::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_W))
			this->m_CameraPosition.y += this->m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_S))
			this->m_CameraPosition.y -= this->m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_D))
			this->m_CameraPosition.x += this->m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_A))
			this->m_CameraPosition.x -= this->m_CameraTranslationSpeed * ts;

		if (this->m_Rotation)
		{
			if (Input::IsMouseButtonPressed(HZ_KEY_Q))
				this->m_CameraRotation += this->m_CameraRotationSpeed * ts;
			if (Input::IsMouseButtonPressed(HZ_KEY_E))
				this->m_CameraRotation -= this->m_CameraRotationSpeed * ts;
			this->m_Camera.SetRotation(m_CameraRotation);
		}
		this->m_Camera.SetPosition(m_CameraPosition);

		this->m_CameraTranslationSpeed = m_ZoomLevel;
	}
	
	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthoGraphicCameraController::OnWindowResized));
	}
	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		this->m_ZoomLevel -= e.GetYOffset() * 0.25f;
		this->m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		this->m_Camera.SetProjection(-this->m_AspectRatio * this->m_ZoomLevel, this->m_AspectRatio * this->m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoGraphicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		this->OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}

