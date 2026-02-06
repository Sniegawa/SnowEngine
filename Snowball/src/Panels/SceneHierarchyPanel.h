#pragma once

#include <Core/Scene/Scene.h>
#include <Core/Scene/Entity.h>
#include <Core/Scene/Components.h>

#include <filesystem>

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

		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }
		Entity GetSelectedEntity() const { return m_SelectionContext; };

		void SetAssetsPath(std::filesystem::path path) { m_AssetsPath = path; }

	private:
		void DrawEntityNode(Entity entity, Entity* entityToDestroy);
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
	private:
		Ref<Scene> m_Context = nullptr;
		Entity m_SelectionContext;
		std::filesystem::path m_AssetsPath;
	};
};
