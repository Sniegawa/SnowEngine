#pragma once

#include <Core/Scene/Scene.h>
#include <Core/Scene/Entity.h>
#include <Core/Scene/Components.h>
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

		Entity GetSelectedEntity() const { return m_SelectionContext; };

	private:
		void DrawEntityNode(Entity entity, Entity* entityToDestroy);
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
	private:
		Ref<Scene> m_Context = nullptr;
		Entity m_SelectionContext;
	};
};
