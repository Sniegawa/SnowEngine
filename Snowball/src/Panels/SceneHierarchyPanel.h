#pragma once

#include <Core/Scene/Scene.h>

namespace Snow
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void DrawComponents(Entity entity);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context = nullptr;
		Entity m_SelectionContext;
	};
};