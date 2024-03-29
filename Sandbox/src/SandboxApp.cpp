#include "Hazel.h"
#include "Hazel/Core/EntryPoint.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"


class Sandbox :
    public Hazel::Application
{
public:
    Sandbox()
    {
        //PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }
    ~Sandbox()
    {

    }

private:

};
Hazel::Application* Hazel::CreateApplication()
{
    return new Sandbox();
}


class ExampleLayer :
    public Hazel::Layer
{
public:
    ExampleLayer() :
        Layer("Example"),
        m_CameraController(1280.0f / 720.0f, true),
        m_Pos(glm::vec3(1.0f))
    {
        this->m_VertexArray = Hazel::VertexArray::Create();
        float vertices[] =
        {
            -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
        };
        Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
        m_VertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));

        Hazel::BufferLayout layout =
        {
            { "g_Position", Hazel::ShaderDataType::Float3 },
            { "g_Color", Hazel::ShaderDataType::Float4 }
        };
        m_VertexBuffer->SetLayout(layout);
        this->m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t indices[] = { 0, 1, 2 };
        Hazel::Ref<Hazel::IndexBuffer> m_IndexBuffer;
        m_IndexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        this->m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        std::string vertexSrc =
            R"(
            #version 330 core
            layout(location=0) in vec3 a_Position;
            layout(location=1) in vec4 a_Color;
            out vec4 v_Color;
            out vec3 v_Position;
            uniform mat4 projection;
            uniform mat4 view;
            void main()
            {
                v_Position = a_Position;
                gl_Position = projection * view * vec4(a_Position, 1.0f);
                v_Color = a_Color;
            }
        )";

        std::string fragmentSrc =
            R"(
            #version 330 core
            //layout(location=0) out vec3 a_Position;
            layout(location=0) out vec4 color;
            in vec4 v_Color;
            in vec3 v_Position;
            void main()
            {
                color = v_Color;
            }
        )";
        this->shader = Hazel::Shader::Create("basic", vertexSrc, fragmentSrc);

        float quadVertices[] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };
        this->quadVa = Hazel::VertexArray::Create();
        m_VertexBuffer = Hazel::VertexBuffer::Create(quadVertices, sizeof(quadVertices));

        layout = {
            { "g_Position", Hazel::ShaderDataType::Float3 },
            {"g_Texture", Hazel::ShaderDataType::Float2}
        };
        m_VertexBuffer->SetLayout(layout);
        this->quadVa->AddVertexBuffer(m_VertexBuffer);

        uint32_t quadIndices[] = { 0, 1, 2, 0, 3, 2 };
        m_IndexBuffer = Hazel::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
        this->quadVa->SetIndexBuffer(m_IndexBuffer);

        std::string vertexSrc2 =
            R"(
            #version 330 core
            layout(location=0) in vec3 a_Position;
            layout(location=1) in vec2 a_TexCoord;
            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 transform;
            out vec2 v_TexCoord;
            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = projection * view * transform * vec4(a_Position, 1.0f);
            }
        )";

        std::string fragmentSrc2 =
            R"(
            #version 330 core
            out vec4 FragColor;
            in vec2 v_TexCoord;
            uniform sampler2D tex;

            void main()
            {
                FragColor = texture(tex, v_TexCoord);
            }
        )";
        //this->quadShader.reset(Hazel::Shader::Create(vertexSrc2, fragmentSrc2));
        this->quadShader = Hazel::Shader::Create("Assets/Shaders/Texture.glsl");

        this->quadTexture = (Hazel::Texture2D::Create((std::string)"Assets/Textures/1.png"));
        this->texture2 = (Hazel::Texture2D::Create((std::string)"Assets/Textures/3.png"));


        std::dynamic_pointer_cast<Hazel::OpenGLShader>(this->quadShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(this->quadShader)->SetUniformInt("tex", 0);

    }

    void OnUpdate(Hazel::TimeStep ts) override
    {

        this->m_CameraController.OnUpdate(ts);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->m_Pos);

        Hazel::RenderCommand::SetClearColor({ 1.0f, 0.1f, 1.0f, 1.0f });
        Hazel::RenderCommand::Clear();

        Hazel::Renderer::BeginScene(this->m_CameraController.GetCamera());

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

        glm::vec4 red = glm::vec4(1.0f, 0.1f, 0.2f, 1.0f);
        glm::vec4 blue = glm::vec4(0.2f, 0.3f, 1.0f, 1.0f);

        for (int i = 0; i < 12; i++)
        {
            for (int j = 0; j < 12; j++)
            {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.11f * j, 0.11f * i, 0.0f)) * model;

                //glm::mat4 transform = glm::translate(model, glm::vec3(0.11f * j, 0.11f * i, 0.0f));
                if (j % 2 == 0)
                    std::dynamic_pointer_cast<Hazel::OpenGLShader>(this->quadShader)->SetUniformFloat4("Color", red);
                else
                    std::dynamic_pointer_cast<Hazel::OpenGLShader>(this->quadShader)->SetUniformFloat4("Color", blue);
                Hazel::Renderer::Submit(this->quadVa, this->quadShader, transform);

            }
        }
        this->quadTexture->Bind();
        Hazel::Renderer::Submit(this->quadVa, this->quadShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        this->texture2->Bind();
        //Hazel::Renderer::Submit(this->m_VertexArray, this->shader);
        Hazel::Renderer::EndScend();

    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
        ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    void OnEvent(Hazel::Event& e) override
    {
        //HZ_TRACE("{0}", e);
        this->m_CameraController.OnEvent(e);
    }
private:
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;
    Hazel::Ref<Hazel::Shader> shader;

    Hazel::Ref<Hazel::VertexArray> quadVa;
    Hazel::Ref<Hazel::Shader> quadShader;
    Hazel::Ref<Hazel::Texture2D> quadTexture, texture2;

    Hazel::OrthographicCameraController m_CameraController;

    glm::vec3 m_Pos;
};