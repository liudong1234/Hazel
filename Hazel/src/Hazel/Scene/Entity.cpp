#include "hzpch.h"
#include "Entity.h"

namespace Hazel
{
    Entity::Entity(entt::entity entity, Scene* scene):
        m_EntityHandle(entity), m_Scene(scene)
    { }
    
}