#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include "Hazel/Scene/Components.h"

namespace Hazel
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        this->m_Context = context;
		this->m_SelectedContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

		for (auto en : this->m_Context->m_Registry.view<entt::entity>())
		{
			Entity entity(en, this->m_Context.get());
			this->DrawEntityNode(entity);
		}
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectedContext = {};
        //右键菜单
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Create Empty Entity"))
                    this->m_Context->CreateEntity("Empty Entity");
                ImGui::EndPopup();
            }
        }
        ImGui::EndChild();

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectedContext)
        {
            this->DrawComponents(m_SelectedContext);
        }
        ImGui::End();

    }
    
    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string name, Entity entity, UIFunction uifunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
        
        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineheight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineheight, lineheight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove Components"))
                    removeComponent = true;
                ImGui::EndPopup();
            }

            if (open)
            {
                //auto& src = entity.GetComponent<T>();
                //ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                uifunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };


        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.9f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.15f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.1f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        ImGui::DragFloat("##z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tc = entity.GetComponent<TagComponent>();
        ImGuiTreeNodeFlags flag = ((m_SelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flag |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tc.Tag.c_str());

        if (ImGui::IsItemClicked())
        {
            this->m_SelectedContext = entity;
        }
        bool deleteEntity = false;

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                deleteEntity = true;
            ImGui::EndPopup();
            //ImGui::OpenPopupOnItemClick("my popup", ImGuiPopupFlags_MouseButtonRight);
        }
        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)324233, flags, tc.Tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
        if (deleteEntity)
        {
            this->m_Context->DestroyEntity(entity);
            if (m_SelectedContext == entity)
                m_SelectedContext = {};
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
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                m_SelectedContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Spirte"))
            {
                m_SelectedContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
            {
                DrawVec3Control("Position", component.Translation);
                auto& rotation = glm::degrees(component.Rotation);
                DrawVec3Control("Rotation", rotation);
                component.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", component.Scale, 1.0f);
            });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
            {
                ImGui::Checkbox("Primary", &component.Primary);
                auto& camera = component.Camera;
                const char* projectionTypeString[] = { "perspective", "ortho" };
                const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
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

                    ImGui::Checkbox("fixed aspect ratio", &component.FixedAspectRatio);
                }
            });
        
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
            });
    }
}
