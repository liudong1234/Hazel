#pragma once
#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Hazel/Renderer/EditorCamera.h"
#include <filesystem>
namespace Hazel
{
    class EditorLayer :
        public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer();
        void OnUpdate(TimeStep ts) override;
        void OnAttach();
        void OnDetach();
        void OnImGuiRender() override;
        void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressEvent& event);
		void NewScene();
		void OpenScene();
		void OpenScene(std::filesystem::path& path);
		void SaveAsScene();

    private:
        Ref<Texture2D> quadTexture;
        Ref<SubTexture2D> subQuad;
        std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
        Ref<Framebuffer> m_Framebuffer;

        OrthographicCameraController m_CameraController;
		EditorCamera m_EditorCamera;
        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2];

        bool m_ViewportFocus;
        bool m_ViewportHover;
		int m_GizmoType;
        SceneHierarchyPanel m_Panel;
		ContentBrowserPanel m_ContentBrowserPanel;
		Entity m_HoveredEntity;

    };

}
