#include "hzpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    SceneCamera::SceneCamera() :
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

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        this->m_AspectRatio = (float)width / (float)height;
        ReCalculateProjection();
    }

    void SceneCamera::ReCalculateProjection()
    {
        float orthoLeft = -this->m_OrthographiSize * this->m_AspectRatio * 0.5f;
        float orthoRight = this->m_OrthographiSize * this->m_AspectRatio * 0.5f;
        float orthoBottom = -this->m_OrthographiSize * 0.5f;
        float orthoTop = this->m_OrthographiSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
            this->m_OrthographicNear, this->m_OrthographicFar);
    }

}
