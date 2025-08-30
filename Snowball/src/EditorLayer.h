#pragma once

#include <SnowEngine/SnowEngine.h>

namespace Snow
{
	class EditorLayer : public Layer
	{
	public:

		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<Framebuffer>m_Framebuffer;
		OrthographicCameraController m_CameraController;
	};
};