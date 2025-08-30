#pragma once

#include <SnowEngineAPI.h>
#include "SnowEngine/Engine/Core/KeyCode.h"
#include "SnowEngine/Engine/Core/MouseCode.h"
#include <utility>

namespace Snow
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);

		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
};
