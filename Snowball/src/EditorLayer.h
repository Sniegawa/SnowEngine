#pragma once

#include <Core/Renderer/Camera/EditorCamera.h>
#include <SnowEngine/SnowEngine.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Tools/ProjectManager.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();

		void CreateNewProject(std::string path, std::string name);
		void OpenProject();
	private:
		Ref<Framebuffer>m_Framebuffer;

		bool m_IsViewportFocused = false;
		bool m_IsViewportHovered = false;
		glm::ivec2 m_ViewportSize = { 0,0 };
		glm::vec2 m_ViewportBounds[2];
		Ref<Scene> m_ActiveScene;
		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity;

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_HierarchyPanel;

		ContentBrowserPanel m_ContentBrowserPanel;

		ProjectManager m_ProjectManager;
		std::filesystem::path m_ActiveScenePath;
	};
};