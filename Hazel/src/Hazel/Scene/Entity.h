#pragma once

#include "Scene.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Log.h"
namespace Hazel
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity entity, Scene* scene);
        Entity(const Entity& ob) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            //HZ_CORE_ASSERT(HasComponent<T>(), "Entity has already component");
            T& component = this->m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
            this->m_Scene->OnComponentAdded(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            //HZ_CORE_ASSERT(!HasComponent<T>(), "Entity does not have component");

            return this->m_Scene->m_Registry.get_or_emplace<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return this->m_Scene->m_Registry.all_of(this->m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

            this->m_Scene->m_Registry.remove<T>(this->m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const{ return (uint32_t)this->m_EntityHandle; }
        operator entt::entity() const{ return this->m_EntityHandle; }

        bool operator == (const Entity& entity) const 
        {
            return this->m_EntityHandle == entity.m_EntityHandle && this->m_Scene == entity.m_Scene;
        }

        bool operator != (const Entity& entity) const
        {
            return !operator == (entity);
        }
    private:
        entt::entity m_EntityHandle = entt::null;
        Scene* m_Scene = nullptr;

    };
}