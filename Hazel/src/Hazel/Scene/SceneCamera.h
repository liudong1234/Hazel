#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
    class SceneCamera:
        public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        float GetOrthographicSize() { return this->m_OrthographiSize; }
        void SetOrthographicSize(float size) { this->m_OrthographiSize = size; ReCalculateProjection(); }

    private:
        void ReCalculateProjection();
    private:
        float m_OrthographiSize;
        float m_OrthographicNear, m_OrthographicFar;

        float m_AspectRatio;
        //glm::mat4 m_Projection;
    };
}