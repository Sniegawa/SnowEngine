#include "Scene.h"

#include "Core/Renderer/Renderer2D.h"

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
		Camera* MainCamera = nullptr;
		glm::mat4* MainCameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComponent,CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
				
				if (camera.Primary)
				{
					MainCamera = &camera.Cam;
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
					auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawQuad(transform.Transform, sprite.Color);
				}
			}

			Renderer2D::EndScene();
		}

		
	}
};