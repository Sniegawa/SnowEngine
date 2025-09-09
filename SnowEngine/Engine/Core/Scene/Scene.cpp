#include "Scene.h"

#include "Core/Renderer/Renderer2D.h"

#include "Core/Audio/AudioSystem.h"

namespace Snow
{
	Scene::Scene()
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
		
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
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
			glm::mat4* MainCameraTransform = nullptr;
			{
				auto view = m_Registry.view<TransformComponent, CameraComponent>();
				for (auto entity : view)
				{
					auto& camera = view.get<CameraComponent>(entity);
					auto& transform = view.get<TransformComponent>(entity);

					if (camera.Primary)
					{
						MainCamera = &camera.Camera;
						MainCameraTransform = &transform.Transform;
						break;
					}
				}
			}

			if (MainCamera)
			{
				Renderer2D::BeginScene(MainCamera->GetProjectionMatrix(), *MainCameraTransform);

				//Render
				{
					auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
					for (auto entity : group)
					{
						const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

						Renderer2D::DrawQuad(transform.Transform, sprite.Color);
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

				AudioSystem::SetListenerPosition(glm::vec3(transform.Transform[3][0], transform.Transform[3][1], transform.Transform[3][2]),listener.ListenerID);
				AudioSystem::SetMasterVolume(listener.masterVolume);
			}
		}


		//Update audio emitters
		{
			{
				auto group = m_Registry.group<SoundEmitterComponent>(entt::get<TransformComponent>);
				for (auto entity : group)
				{
					const auto& [transform, emitter] = group.get<TransformComponent, SoundEmitterComponent>(entity);
					if (!emitter.Instance.expired())
					{
						auto Instance = emitter.Instance.lock();
						AudioSystem::SetSoundPosition(Instance, glm::vec2(transform.Transform[3][0], transform.Transform[3][1]));
						AudioSystem::SetSoundConfig(Instance, emitter.Config);
					}
					//else emitter.isPlaying = false;
					
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
};
