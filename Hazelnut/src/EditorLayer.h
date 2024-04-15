#pragma once
#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"

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
        Ref<Texture2D> quadTexture;
        Ref<SubTexture2D> subQuad;
        std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
        Ref<Framebuffer> m_Framebuffer;

        OrthographicCameraController m_CameraController;
        
        Ref<Scene> m_ActiveScene;
        entt::entity m_SquareEntity;
        glm::vec3 m_Pos;

        glm::vec4 m_Color;

        glm::vec3 m_QuadPos;
        glm::vec2 m_QuadSize;
        float m_QuadAngle;
        glm::vec2 m_ViewportSize;
        bool m_ViewportFocus;
        bool m_ViewportHover;
    };

}