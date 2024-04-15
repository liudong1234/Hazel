#pragma once

#include "entt.hpp"
#include "Hazel/Core/TimeStep.h"

namespace Hazel
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();
        void OnUpdate(TimeStep ts);

        //temp
        entt::registry& Reg() { return m_Registry; }
    private:
        entt::registry m_Registry;//组件和实体的容器

    };
}