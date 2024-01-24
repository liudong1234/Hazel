#include "hzpch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Hazel
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(1.0f),
		m_Camera(-m_AspectRatio * this->m_ZoomLevel, m_AspectRatio* this->m_ZoomLevel, -this->m_ZoomLevel, this->m_ZoomLevel),
		m_CameraRotationSpeed(180.0f), m_CameraTranslationSpeed(5.0f),
		m_CameraPos({ 0.0f, 0.0f, 0.0f }),
		m_CameraRotation(0.0f),
		m_Rotation(rotation)
	{
		HZ_CORE_WARN("{0}", m_ZoomLevel);
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		this->m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnUpdate(TimeStep& ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_W))
			this->m_CameraPos.y += this->m_CameraTranslationSpeed * ts.GetSeconds();
		if (Input::IsKeyPressed(HZ_KEY_S))
			this->m_CameraPos.y -= this->m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_A))
			this->m_CameraPos.x -= this->m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_D))
			this->m_CameraPos.x += this->m_CameraTranslationSpeed * ts;

		if (this->m_Rotation)
		{
			if (Input::IsKeyPressed(HZ_KEY_Q))
				this->m_CameraRotation -= this->m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_E))
				this->m_CameraRotation += this->m_CameraRotationSpeed * ts;
			this->m_Camera.SetRotation(this->m_CameraRotation);
		}

		this->m_Camera.SetPosition(this->m_CameraPos);
		this->m_CameraTranslationSpeed = this->m_ZoomLevel;
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::MouseEventScrolled));

		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::WindowResized));

	}
	bool OrthographicCameraController::MouseEventScrolled(MouseScrolledEvent& e)
	{
		this->m_ZoomLevel -= e.GetYOffset() * 0.25f;
		this->m_ZoomLevel = std::max(this->m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::WindowResized(WindowResizeEvent& e)
	{
		this->OnResize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}
}