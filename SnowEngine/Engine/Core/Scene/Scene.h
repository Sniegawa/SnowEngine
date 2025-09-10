#pragma once

#include <string>
#include <entt.hpp>

#include "Components.h"
#include "Core/Timestep.h"
#include "Entity.h"

namespace Snow
{

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "New Entity");
		
		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		friend class Entity;
		friend class SceneHierarchyPanel;
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	};
}