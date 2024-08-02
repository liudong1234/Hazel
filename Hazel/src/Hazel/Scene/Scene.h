#pragma once

#include "entt.hpp"
#include "Hazel/Core/TimeStep.h"
#include "Hazel/Renderer/EditorCamera.h"

class b2World;

namespace Hazel
{
    class Entity;
    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

		void OnUpdateStart();
		void OnUpdateStop();

        void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
        void OnUpdateRuntime(TimeStep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCamera();

    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_Registry;//组件和实体的容器
        uint32_t m_ViewportWidth, m_ViewportHeight;

		b2World* m_PhysicsWorld;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}
