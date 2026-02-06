#include "Scene.h"

#include "Entity.h"
#include "Components.h"
#include "Core/Renderer/Renderer2D.h"

#include "Core/Audio/AudioSystem.h"
#include "Core/Scene/Components.h"



namespace Snow
{
	Scene::Scene(std::string name)
		: m_SceneName(name)
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag = name;
		auto id = GenerateUUID();
		entity.AddComponent<UUIDComponent>(id);

		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name, const UUID id)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag = name;
		auto& idComp = entity.AddComponent<UUIDComponent>();
		idComp.id = id;

		return entity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity,this };
		}
		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts,EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		//Render scene on editor camera
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}


	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=,this](auto entity, NativeScriptComponent& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity,this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});

		}

		//Render2D
		{
			Camera* MainCamera = nullptr;
			glm::mat4 MainCameraTransform;
			{
				auto view = m_Registry.view<TransformComponent, CameraComponent>();
				for (auto entity : view)
				{
					auto& camera = view.get<CameraComponent>(entity);
					auto& transform = view.get<TransformComponent>(entity);

					if (camera.Primary)
					{
						MainCamera = &camera.Camera;
						MainCameraTransform = transform.GetTransform();
						break;
					}
				}
			}

			if (MainCamera)
			{
				Renderer2D::BeginScene(MainCamera->GetProjectionMatrix(), MainCameraTransform);

				//Render
				{
					auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
					for (auto entity : group)
					{
						const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
					}
				}

				Renderer2D::EndScene();
			}
		}

		//Update audio listener
		{
			auto view = m_Registry.view<TransformComponent, AudioListenerComponent>();
			for (auto entity : view)
			{
				const auto& [transform, listener] = view.get<TransformComponent, AudioListenerComponent>(entity);

				AudioSystem::SetListenerPosition(glm::vec3(transform.Translation.x, transform.Translation.y, transform.Translation.z),listener.ListenerID);
				AudioSystem::SetMasterVolume(listener.masterVolume);
			}
		}


		//Update audio emitters
		{
			{
				auto group = m_Registry.group<AudioEmitterComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					const auto& [transform, emitter] = group.get<TransformComponent, AudioEmitterComponent>(entity);
					if (!emitter.Instance.expired())
					{
						auto Instance = emitter.Instance.lock();
						AudioSystem::SetAudioPosition(Instance, transform.Translation);
						AudioSystem::SetAudioConfig(Instance, emitter.Config);
					}
					else emitter.isPlaying = false;
					
				}
			}
		}
			
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}

	}
	//TODO : think about reworking this to use some kind of component ID system
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		SNOW_CORE_ASSERT(false, "There is no OnComponentAdded specification for given component");
	}

	template<>
	void Scene::OnComponentAdded<UUIDComponent>(Entity entity, UUIDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<AudioListenerComponent>(Entity entity, AudioListenerComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<AudioEmitterComponent>(Entity entity, AudioEmitterComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
};
