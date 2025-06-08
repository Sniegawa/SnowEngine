#pragma once

#include "../../SnowEngineAPI.h"

#include <string>
#include <functional>
#include <map>


namespace SnowEngine
{
	enum EventType
	{
		NONE = 0,

		WindowResize,
		WindowClose,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,

		AppTick,
		AppUpdate,
		AppReder,

		KeyPressed,
		KeyReleased,
		KeyTyped,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled,

	};

	enum EventCategory
	{
		EVENT_NONE = 0,
		EVENT_CATEGORY_APP			= BIT(0),
		EVENT_CATEGORY_INPUT		= BIT(1),
		EVENT_CATEGORY_MOUSE		= BIT(2),
		EVENT_CATEGORY_MOUSEBUTTON	= BIT(3),
		EVENT_CATEGORY_KEYBOARD		= BIT(4),
		EVENT_CATEGORY_WINDOW		= BIT(5),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;}\
								virtual EventType GetEventType() const override {return GetStaticType();}\
								virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category;}


	class SNOW_API Event
	{
		friend class EventDispatcher;
	public:

		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0; //For debug
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		const bool Handled() const { return m_handled; }

		inline  bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_handled = false;
	};
	
	class SNOW_API EventDispatcher
	{
	private:
		template<typename T>
		using Func = std::function<bool(T&)>; // Listening function to event of type T

		Event& m_event;
	public:
		
		EventDispatcher(Event& event) : m_event(event) {}

		template<typename T>
		bool Dispatch(Func<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.m_handled = func(*(T*)&m_event); //Magic hack for casting event to T and using it as argument
				return true;
			}
			return false;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

};// namespace SnowEngine