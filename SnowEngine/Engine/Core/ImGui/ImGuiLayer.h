#pragma once

#include "../Layers/Layer.h"

#include "SnowEngine/Engine/SnowEngineAPI.h"

#include "../Events/ApplicationEvent.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"

namespace Snow
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual	void OnAttach() override;
		virtual	void OnDetach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();

	private:

		float m_Time = 0.0f;
	};

	
};