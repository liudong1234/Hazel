#pragma once
#include "Scene.h"
#include "Components.h"
#include "Hazel/Core/Log.h"

namespace Hazel
{
    class Entity
    {
    public:
		Entity() = default;
		Entity(entt::entity handle, Scene * scene);
		Entity(const Entity & other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

        template<typename T>
        T& GetComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
            return this->m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return this->m_Scene->m_Registry.all_of<T>(this->m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

            this->m_Scene->m_Registry.remove<T>(this->m_EntityHandle);
        }
		
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const{ return (uint32_t)m_EntityHandle; }
		//这里遇到的问题是，由于自身习惯，类内成员会加上this,以至于在强制转换时没有用括号确定好范围
		// 导致了很难发现的错误！！！
        //operator uint32_t() const{ return (uint32_t)this->m_EntityHandle; }
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
