#pragma once

#include <Core/Renderer/Camera/EditorCamera.h>
#include <SnowEngine/SnowEngine.h>

#include "Panels/SceneHierarchyPanel.h"
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
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Ref<Framebuffer>m_Framebuffer;
		OrthographicCameraController m_CameraController; //TODO : Remove

		bool m_IsViewportFocused = false;
		bool m_IsViewportHovered = false;
		glm::ivec2 m_ViewportSize = { 0,0 };

		Ref<Scene> m_ActiveScene;
 
		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_Hierarchy;

	};
};