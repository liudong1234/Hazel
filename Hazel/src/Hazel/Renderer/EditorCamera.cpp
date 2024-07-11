#include "hzpch.h"
#include "EditorCamera.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Hazel
{
	EditorCamera::EditorCamera():
		m_Fov(45.0f), m_AspectRatio(1.778f), m_NearClip(0.1f), m_FarClip(1000.0f),
		m_Position({0.0f, 0.0f, -10.0f}), m_FocalPoint({0.0f, 0.0f, 0.0f}),
		m_Distance(10.0f), m_Pitch(0.0f), m_Yaw(0.0f),
		m_ViewportWidth(1280.0f), m_ViewportHeight(720.0f),
		m_InitialMousePositon(0.0f), m_ViewMatrix(glm::mat4(1.0f))
	{
	}

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip):
		m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
		Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		this->UpdateView();
	}

	EditorCamera::~EditorCamera()
	{
	}

	void EditorCamera::UpdateProjection()
	{
		this->m_AspectRatio = this->m_ViewportWidth / this->m_ViewportHeight;
		this->m_Projection = glm::perspective(glm::radians(this->m_Fov), this->m_AspectRatio, this->m_NearClip, this->m_FarClip);
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_LEFT_ALT))
		{
			const glm::vec2& mouse {Input::GetMouseX(), Input::GetMouseY()};
			glm::vec2 delta = (mouse - this->m_InitialMousePositon) * 0.003f;
			this->m_InitialMousePositon = mouse;

			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_MIDDLE))
				this->MousePan(delta);
			else if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
				this->MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
				this->MouseZoom(delta.y);
		}
		this->UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		this->MouseZoom(delta);
		this->UpdateView();
		return false;//?
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = this->PanSpeed();
		this->m_FocalPoint += -this->GetRightDirection() * delta.x * xSpeed * this->m_Distance;
		this->m_FocalPoint += this->GetUpDirection() * delta.y * ySpeed * this->m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = this->GetUpDirection().y < 0 ? -1.0f : 1.0f;
		this->m_Yaw += yawSign * delta.x * this->RotationSpeed();
		this->m_Pitch += delta.y * this->RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		this->m_Distance -= delta * this->ZoomSpeed();
		if (this->m_Distance < 1.0f)
		{
			this->m_FocalPoint += this->GetForwardDirection();
			this->m_Distance = 1.0f;
		}
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-this->m_Pitch, -this->m_Yaw, 0.0f));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(this->GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(this->GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(this->GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void EditorCamera::UpdateView()
	{
		//this->m_Yaw = this->m_Pitch = 0.0f;//使得相机不可选择，即完全的2D
		this->m_Position = CalculaePosition();

		glm::quat orientation = GetOrientation();
		this->m_ViewMatrix = glm::translate(glm::mat4(1.0f), this->m_Position) * glm::toMat4(orientation);
		this->m_ViewMatrix = glm::inverse(this->m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(this->m_ViewportWidth / 1000.0f, 2.4f);
		float xfactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(this->m_ViewportHeight / 1000.0f, 2.4f);
		float yfactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
		return { xfactor, yfactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = this->m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);

		return speed;
	}

	glm::vec3 EditorCamera::CalculaePosition() const
	{
		return this->m_FocalPoint - this->GetForwardDirection() * this->m_Distance;
	}
}
