#pragma once

#include "Event.h"

namespace Hazel
{
	class HAZEL_API KeyEvent :
		public Event
	{
	public:
		inline int GetKeyCode() const { return this->m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class HAZEL_API KeyPressEvent :
		public KeyEvent
	{
	public:
		KeyPressEvent(int keycode, int repeatCount) :
			KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPresssEvent: " << this->m_KeyCode << "(" << this->m_RepeatCount << "repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class HAZEL_API KeyReleaseEvent :
		public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << this->m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class HAZEL_API KeyTypeEvent :
		public KeyEvent
	{
	public:
		KeyTypeEvent(int keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypeEvent: " << this->m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyType)
	};
}