#pragma once

#include "SnowEngine/Engine/Core/Input.h"

namespace Snow
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCode code) override;
		virtual bool IsMouseButtonPressedImpl(MouseCode code) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
};