#pragma once

#include <entt.hpp>
#include <SnowEngineAPI.h>

namespace Snow
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) { }

		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.template all_of<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			SNOW_CORE_ASSERT(!HasComponent<T>(), "Entity already has given component");
			T& component = m_Scene->m_Registry.template emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this,component);
			return component;
		}
		
		template<typename T>
		T& GetComponent()
		{
			SNOW_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have given component");
			return m_Scene->m_Registry.template get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			SNOW_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have given component");
			m_Scene->m_Registry.template remove<T>(m_EntityHandle);
		}

		operator bool() { return m_EntityHandle != entt::null; }
		operator uint32_t() { return (uint32_t)m_EntityHandle; }
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
		operator entt::entity() { return m_EntityHandle; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

};
