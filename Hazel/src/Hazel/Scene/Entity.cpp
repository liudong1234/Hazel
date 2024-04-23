#include "hzpch.h"
#include "Entity.h"

namespace Hazel
{
    Entity::Entity():
        m_EntityHandle(entt::null), m_Scene(nullptr)
    {
    }

    Entity::Entity(entt::entity entity, Scene* scene):
        m_EntityHandle(entity), m_Scene(scene)
    { }
    
}