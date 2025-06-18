#pragma once

#include "SnowEngine/Engine/SnowEngineAPI.h"
#include "SnowEngine/Engine/Core/KeyCode.h"
#include "SnowEngine/Engine/Core/MouseCode.h"
#include <utility>

namespace Snow
{
	class Input
	{
	public:
		inline static bool IsKeyPressed(KeyCode code) { return s_Instance->IsKeyPressedImpl(code); }

		inline static bool IsMouseButtonPressed(MouseCode code) { return s_Instance->IsMouseButtonPressedImpl(code); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(KeyCode code) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode code) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};
};