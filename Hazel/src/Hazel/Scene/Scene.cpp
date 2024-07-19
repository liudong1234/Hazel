#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Hazel
{
	Scene::Scene() :
		m_ViewportHeight(0), m_ViewportWidth(0)
	{
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { this->m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		this->m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			//Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, int(entity));
		}
		Renderer2D::EndScene();
	}

    void Scene::OnUpdateRuntime(TimeStep ts)
    {
        //update scripts
        {
            this->m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity, this };
                        nsc.Instance->OnCreate();
                    }
                    nsc.Instance->OnUpdate(ts);
                });

        }

        //render sprites
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;

        {
            auto views = this->m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : views)
            {
                //auto& transform = views.get<TransformComponent>(entity);
                //auto& camera = views.get<CameraComponent>(entity);
                auto [transform, camera] = views.get<TransformComponent, CameraComponent>(entity);
                
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            
            }
        }
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
            auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
                //Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            Renderer2D::EndScene();

        }
    }


    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        this->m_ViewportWidth = width;
        this->m_ViewportHeight = height;

        auto view = this->m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

	Entity Scene::GetPrimaryCamera()
	{
		auto views = this->m_Registry.view<CameraComponent>();
		for (auto entity : views)
		{
			const auto& cc = this->m_Registry.get<CameraComponent>(entity);
			if (cc.Primary)
				return Entity{ entity, this };
		}

		return {};
	}


    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
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
        if (this->m_ViewportWidth > 0 && this->m_ViewportHeight > 0)
            component.Camera.SetViewportSize(this->m_ViewportWidth, this->m_ViewportHeight);
    }
    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }
}
