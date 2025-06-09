#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"
#include "SnowEngine/Engine/Core/MouseCode.h"

#include <string>

#include <glm/glm.hpp>

namespace Snow
{

	class SNOW_API MouseMovedEvent : public Event
	{
	public:

		MouseMovedEvent(float x, float y) : m_MousePos(x, y) {}

		float GetX() const { return m_MousePos.x; }
		float GetY() const { return m_MousePos.y; }
		glm::vec2 GetPosition() const { return m_MousePos; }

		std::string ToString() const override
		{
			return "MouseMovedEvent: " + std::to_string(m_MousePos.x) + ", " + std::to_string(m_MousePos.y);
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);

	private:
		glm::vec2 m_MousePos;
	};

	class SNOW_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			return "MouseScrolledEvent: " + std::to_string(m_XOffset) + ", " + std::to_string(m_YOffset);
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);

	private:
		float m_XOffset, m_YOffset;

	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
	protected:
		MouseButtonEvent(MouseCode button) : m_Button(button) {}
		MouseCode m_Button;
	};

	class SNOW_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}
		std::string ToString() const override
		{
			return "MouseButtonPressedEvent: " + std::to_string(m_Button);
		}
		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class SNOW_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}
		std::string ToString() const override
		{
			return "MouseButtonReleasedEvent: " + std::to_string(m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);

	};
};