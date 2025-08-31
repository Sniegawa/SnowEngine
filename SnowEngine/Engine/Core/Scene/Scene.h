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
	private:
		friend class Entity;
		entt::registry m_Registry;
	};
}