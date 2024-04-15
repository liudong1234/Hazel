#include "hzpch.h"
#include "Scene.h"
#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel
{
    void DoMath(const glm::mat4& transform)
    {

    }

    Scene::Scene()
    {
        //TransformComponent transform;
        //DoMath(transform);
    #if 0
        entt::entity entity = this->m_Registry.create();

        this->m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

        this->m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

        if (this->m_Registry.has<TransformComponent>(entity))//?
            TransformComponent& transform = this->m_Registry.get<TransformComponent>(entity);

        auto view = this->m_Registry.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(entity);

        }

        auto group = this->m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto&[transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
        }
    #endif
    }

    Scene::~Scene()
    {

    }

    entt::entity Scene::CreateEntity()
    {
        return this->m_Registry.create();
    }

    void Scene::OnUpdate(TimeStep ts)
    {
        auto group = this->m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);

        }
    }
}