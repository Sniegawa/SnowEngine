#pragma once

#include "Event.h"
#include "../KeyCode.h"
#include <string>

namespace SnowEngine
{
	class KeyEvent : public Event
	{
	protected:

		KeyEvent(KeyCode KeyCode) : m_KeyCode(KeyCode) {}
	private:
		KeyCode m_KeyCode;
	public:

		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);

	};
	
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode KeyCode, int repeatCount) : KeyEvent(KeyCode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			return "KeyPressedEvent: " + std::to_string(GetKeyCode()) + " (" + std::to_string(m_RepeatCount) + " repeats)";
		}

		EVENT_CLASS_TYPE(KeyPressed);


	private:
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode KeyCode) : KeyEvent(KeyCode) {}

		std::string ToString() const override
		{
			return "KeyReleasedEvent: " + std::to_string(GetKeyCode());
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode KeyCode) : KeyEvent(KeyCode) {}
		std::string ToString() const override
		{
			return "KeyTypedEvent: " + std::to_string(GetKeyCode());
		}
		EVENT_CLASS_TYPE(KeyTyped);
	};
};