#pragma once

#include "SnowEngine/Engine/Core/Events/Event.h"

#include "SnowEngine/Engine/SnowEngineAPI.h"
#include <sstream>

namespace Snow
{
	class Layer
	{
	public:
		Layer(const std::string& name) : m_Name(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_Name; }
	protected:
		std::string m_Name;
	};
}