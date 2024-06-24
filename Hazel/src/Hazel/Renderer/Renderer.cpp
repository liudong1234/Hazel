#include "hzpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    void Renderer::Init()
    {
        HZ_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }
    void Renderer::OnResize(uint16_t width, uint16_t height)
    {
        RenderCommand::SetViewPort(0, 0, width, height);
    }
    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
        m_SceneData->viewMatrix = camera.GetViewMatrix();
        m_SceneData->projectionMatrix = camera.GetProjectionMatrix();
    }

    void Renderer::EndScend()
    {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4 transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformMat4("projection", m_SceneData->projectionMatrix);
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformMat4("view", m_SceneData->viewMatrix);
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(shader)->SetUniformMat4("transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}