#pragma once

#include <string>
#include <entt.hpp>

#include <SnowEngineAPI.h>
#include "Core/Timestep.h"
#include <Core/Renderer/Camera/EditorCamera.h>
#include "Utilities/UUID.h"

namespace Snow
{
  class Entity;

	class Scene
	{
	public:
		Scene(std::string name = "New Scene");
		~Scene();

		Entity CreateEntity(const std::string& name = "New Entity");
		Entity CreateEntity(const std::string& name, const UUID id);
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts,EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		std::string m_SceneName;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
