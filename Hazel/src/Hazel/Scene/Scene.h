#pragma once

#include "entt.hpp"
#include "Hazel/Core/TimeStep.h"

namespace Hazel
{
    class Entity;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(std::string name = std::string());
        void OnUpdate(TimeStep ts);

        void OnViewportResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_Registry;//组件和实体的容器
        uint32_t m_ViewportWidth, m_ViewportHeight;

        friend class Entity;
        friend class SceneHierarchyPanel;
    };
}