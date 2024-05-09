#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
namespace Hazel
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        this->m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Entity");
        
        for (auto en : this->m_Context->m_Registry.view<entt::entity>()) 
        {
            Entity entity(en, this->m_Context.get());
            this->DrawEntityNode(entity);
        }
        ImGui::End();
       
        ImGui::Begin("Properties");
        if (m_SelectedContext)
        {
            this->DrawComponents(m_SelectedContext);
        }
        ImGui::End();

    }

    void SceneHierarchyPanel::DrawEntityNode(Entity& entity)
    {
        auto& tc = entity.GetComponent<TagComponent>();
        ImGuiTreeNodeFlags flag = ((m_SelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tc.Tag.c_str());

        if (ImGui::IsItemClicked())
        {
            this->m_SelectedContext = entity;
        }

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            this->m_SelectedContext = {};

        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)324233, flags, tc.Tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }
    
    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;
                
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (entity.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.15f);
                ImGui::TreePop();
            }
        }
        auto a = entity.HasComponent<CameraComponent>();
        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& camera = entity.GetComponent<CameraComponent>().Camera;

                const char* projectionTypeString[] = { "perspective", "ortho" };
                const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

                if (ImGui::BeginCombo("camera", currentProjectionTypeString))
                {
                    for (int i = 0; i < sizeof(projectionTypeString) / sizeof(char*); i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeString[i];
                        if (ImGui::Selectable(projectionTypeString[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeString[i];
                            camera.SetProjectionType(SceneCamera::ProjectionType(i));
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Prospective)
                {
                    float fari = camera.GetPerspectiveFar();
                    float neari = camera.GetPerspectiveNear();
                    float FOV = glm::degrees(camera.GetPerspectiveVerticalFOV());

                    if (ImGui::DragFloat("FOV", &FOV))
                        camera.SetPerspectiveVerticalFOV(glm::radians(FOV));
                    if (ImGui::DragFloat("farClip", &fari))
                        camera.SetPerspectiveFar(fari);
                    if (ImGui::DragFloat("nearClip", &neari))
                        camera.SetPerspectiveNear(neari);
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float fari = camera.GetOrthographicFar();
                    float neari = camera.GetOrthographicNear();
                    float size = camera.GetOrthographicSize();

                    if (ImGui::DragFloat("size", &size))
                        camera.SetOrthographicSize(size);
                    if (ImGui::DragFloat("farClip", &fari))
                        camera.SetOrthographicFar(fari);
                    if (ImGui::DragFloat("nearClip", &neari))
                        camera.SetOrthographicNear(neari);
                }


                ImGui::TreePop();
            }
        }

    }
}