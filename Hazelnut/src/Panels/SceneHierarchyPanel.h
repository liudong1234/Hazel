#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
namespace Hazel
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
		Entity GetSelectedEntity() const { return this->m_SelectedContext; }

    private:
        void DrawEntityNode(Entity entity);

        void DrawComponents(Entity entity);

    private:

        Ref<Scene> m_Context;
        Entity m_SelectedContext;
    };
}
