#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
    class SceneCamera:
        public Camera
    {
    public:
        enum class ProjectionType
        {
            Prospective = 0, Orthographic
        };

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        void SetPerspective(float vercialFOV, float nearClip, float farClip);
        float GetPerspectiveVerticalFOV() { return this->m_PerspectiveFOV; }
        void SetPerspectiveVerticalFOV(float size) { this->m_PerspectiveFOV = size; ReCalculateProjection(); }
        float GetPerspectiveFar() { return this->m_PerspectiveFar; }
        void SetPerspectiveFar(float farClip) { this->m_PerspectiveFar = farClip; ReCalculateProjection(); }
        float GetPerspectiveNear() { return this->m_PerspectiveNear; }
        void SetPerspectiveNear(float nearClip) { this->m_PerspectiveNear = nearClip; ReCalculateProjection(); }


        float GetOrthographicSize() { return this->m_OrthographiSize; }
        void SetOrthographicSize(float size) { this->m_OrthographiSize = size; ReCalculateProjection(); }
        float GetOrthographicFar() { return this->m_OrthographicFar; }
        void SetOrthographicFar(float fari) { this->m_OrthographicFar = fari; ReCalculateProjection(); }
        float GetOrthographicNear() { return this->m_OrthographicNear; }
        void SetOrthographicNear(float neari) { this->m_OrthographicNear = neari; ReCalculateProjection(); }

        ProjectionType GetProjectionType() { return this->m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { this->m_ProjectionType = type; ReCalculateProjection(); }
    private:
        void ReCalculateProjection();
    private:
        ProjectionType m_ProjectionType;

        float m_PerspectiveFOV;
        float m_PerspectiveNear, m_PerspectiveFar;

        float m_OrthographiSize;
        float m_OrthographicNear, m_OrthographicFar;

        float m_AspectRatio;
        glm::mat4 m_Projection;
    };
}