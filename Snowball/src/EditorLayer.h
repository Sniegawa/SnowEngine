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

		bool m_IsViewportFocused = false;
		bool m_IsViewportHovered = false;
		glm::ivec2 m_ViewportSize = { 0,0 };

		Ref<Scene> m_ActiveScene;
	};
};