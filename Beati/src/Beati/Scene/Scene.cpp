#include "bepch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Beati/Renderer/Renderer2D.h"

namespace Beati
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		// m_EntityMap[uuid] = entity;
		return entity;
	}

	void Scene::OnUpdate(Timestep delta)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}
		
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto grup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : grup)
			{
				auto& [transform, sprite] = grup.get<TransformComponent, SpriteRendererComponent>(entity);
			
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}
}
