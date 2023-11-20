#pragma once
#include "Event.h"

#include <sstream>

namespace Hazel
{
	class HAZEL_API MouseMovedEvent :
		public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			:m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return this->m_MouseX; }
		inline float GetY() const { return this->m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << this->m_MouseX << ", " << this->m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX;
		float m_MouseY;
	};

	class HAZEL_API MouseScrolledEvent :
		public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			:m_XOffset(xoffset), m_YOffset(yoffset) {}

		inline float GetXOffset() const { return this->m_XOffset; }
		inline float GetYOffset() const { return this->m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScolledEvent: " << this->GetXOffset() << ", " << this->GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset;
		float m_YOffset;
	};

	class HAZEL_API MouseButtonEvent :
		public Event
	{
	public:
		inline int GetMouseButton() const { return this->m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) :
			m_Button(button)
		{}
		int m_Button;
	};

	class HAZEL_API MouseButtonPressEvent :
		public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int button) :
			MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << this->m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HAZEL_API MouseButtonReleaseEvent :
		public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button) :
			MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << this->m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}