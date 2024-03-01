#include "hzpch.h"
#include "FPSCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel
{
	FPSCameraController::FPSCameraController(const float aspect) :
		m_Camera(),
		m_CameraPosition(0.0f, 0.0f, 0.0f),
		m_CameraTranslationSpeed(3.0f),
		m_ZoomLevel(1.0f),
		m_Aspect(aspect)
	{
		this->m_Camera.SetProjectionMatrix(aspect, 0.1f, 100.0f);
	}

	void FPSCameraController::OnResize(float width, float height)
	{

	}
	
	void FPSCameraController::OnUpdate(TimeStep ts)
	{
		float time = ts.GetSeconds();
		if (Input::IsKeyPressed(HZ_KEY_W))
			this->m_CameraPosition += this->m_Camera.GetUp() * this->m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_S))
			this->m_CameraPosition -= this->m_Camera.GetUp() * this->m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_D))
			this->m_CameraPosition += this->m_Camera.GetRight() * this->m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_A))
			this->m_CameraPosition -= this->m_Camera.GetRight() * this->m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_Q))
			this->m_CameraPosition -= this->m_Camera.GetFront() * this->m_CameraTranslationSpeed * time;
		if (Input::IsKeyPressed(HZ_KEY_E))
			this->m_CameraPosition += this->m_Camera.GetFront() * this->m_CameraTranslationSpeed * time;

		this->m_Camera.SetPosition(this->m_CameraPosition);
		this->m_CameraTranslationSpeed = this->m_ZoomLevel;
	}
	
	void FPSCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(FPSCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(FPSCameraController::OnWindowResized));
	}
	
	bool FPSCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		this->m_ZoomLevel -= e.GetYOffset() * 0.1f;
		this->m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		
		this->m_Camera.SetZoom(m_ZoomLevel);
		this->m_Camera.SetProjectionMatrix(this->m_Aspect, 0.1f, 100.0f);

		return false;
	}
	
	bool FPSCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		this->OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
