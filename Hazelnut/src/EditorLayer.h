#pragma once
#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"
#include "Panels/SceneHierarchyPanel.h"

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
		void NewScene();
		void OpenScene();
		void SaveAsScene();

    private:
        Ref<Texture2D> quadTexture;
        Ref<SubTexture2D> subQuad;
        std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
        Ref<Framebuffer> m_Framebuffer;

        OrthographicCameraController m_CameraController;
        
        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        bool m_PrimaryCamera;

        glm::vec3 m_Pos;
        glm::vec4 m_Color;
        glm::vec3 m_QuadPos;
        glm::vec2 m_QuadSize;
        float m_QuadAngle;
        glm::vec2 m_ViewportSize;
        bool m_ViewportFocus;
        bool m_ViewportHover;

        SceneHierarchyPanel m_Panel;

    };

}
