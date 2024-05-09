#include "hzpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    SceneCamera::SceneCamera() :
        m_ProjectionType(ProjectionType::Orthographic), 
        m_PerspectiveFOV(glm::radians(45.0f)), m_PerspectiveFar(-1.0f), m_PerspectiveNear(1000.0f),
        m_OrthographiSize(10.0f), m_OrthographicFar(1.0f), m_OrthographicNear(-1.0f)
    {
        ReCalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        this->m_OrthographiSize = size;
        this->m_OrthographicNear = nearClip;
        this->m_OrthographicFar = farClip;

        ReCalculateProjection();
    }

    void SceneCamera::SetPerspective(float vercialFOV, float nearClip, float farClip)
    {
        this->m_ProjectionType = ProjectionType::Prospective;
        this->m_PerspectiveFOV = vercialFOV;
        this->m_PerspectiveFar = farClip;
        this->m_PerspectiveNear = nearClip;
        ReCalculateProjection();

    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        this->m_AspectRatio = (float)width / (float)height;
        ReCalculateProjection();
    }

    void SceneCamera::ReCalculateProjection()
    {
        if (this->m_ProjectionType == ProjectionType::Orthographic)
        {
            float orthoLeft = -this->m_OrthographiSize * this->m_AspectRatio * 0.5f;
            float orthoRight = this->m_OrthographiSize * this->m_AspectRatio * 0.5f;
            float orthoBottom = -this->m_OrthographiSize * 0.5f;
            float orthoTop = this->m_OrthographiSize * 0.5f;

            m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
                this->m_OrthographicNear, this->m_OrthographicFar);
        }
        else if (this->m_ProjectionType == ProjectionType::Prospective)
        {
            this->m_Projection = glm::perspective(this->m_PerspectiveFOV, this->m_AspectRatio, this->m_PerspectiveNear, this->m_PerspectiveFar);
        }
    }

}
