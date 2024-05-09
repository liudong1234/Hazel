#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Hazel
{
    Scene::Scene():
        m_ViewportHeight(0), m_ViewportWidth(0)
    {
    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(std::string name)
    {
        Entity entity = { this->m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag = name.empty() ? "Entity" : name;
        
        return entity;
    }

    void Scene::OnUpdate(TimeStep ts)
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
        glm::mat4* cameraTransform{ nullptr };

        {
            auto views = this->m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : views)
            {
                auto& transform = views.get<TransformComponent>(entity);
                auto& camera = views.get<CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            
            }
        }
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);
            auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform, sprite.Color);
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


}