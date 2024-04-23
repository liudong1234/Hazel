#pragma once

#include "Scene.h"

namespace Hazel
{
    class Entity
    {
    public:
        Entity();
        Entity(entt::entity entity, Scene* scene);
        Entity(Entity& ob) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            //HZ_CORE_ASSERT(!HasComponent<T>(), "Entity has already component");
            return this->m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            //HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

            return this->m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return this->m_Scene->m_Registry.orphan(this->m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            //HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

            this->m_Scene->m_Registry.remove<T>(this->m_EntityHandle);
        }

    private:
        entt::entity m_EntityHandle;
        Scene* m_Scene;

    };
}