#pragma once

#include "../Layers/Layer.h"

#include "SnowEngine/Engine/SnowEngineAPI.h"

#include "../Events/ApplicationEvent.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"

namespace Snow
{
	enum ImGuiTheme
	{
		DeepIce = 0,
		WhiteSnow = 1
	};

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual	void OnAttach() override;
		virtual	void OnDetach() override;
		void Begin();
		void End();

		static void SetTheme(ImGuiTheme theme);

	private:

		float m_Time = 0.0f;
	};

	
};