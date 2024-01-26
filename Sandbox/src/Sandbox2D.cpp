#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D():
	Hazel::Layer("sandbox2d"),
	m_CameraController(640.0f / 360.0f)
{
	//Õý·½ÐÎ
	float quadVertices[] =
	{
		//position		
		-0.7f, -0.7f, 0.0f, 0.0f, 0.0f,
		 0.7f, -0.7f, 0.0f, 1.0f, 0.0f,
		 0.7f,  0.7f, 0.0f, 1.0f, 1.0f,
		-0.7f,  0.7f, 0.0f, 0.0f, 1.0f
	};
	uint32_t quadIndices[] = { 0, 1, 2, 0, 2, 3 };
	Hazel::Ref<Hazel::VertexBuffer> m_QuadBuffer;
	Hazel::Ref<Hazel::IndexBuffer> m_QuadIndex;

	this->quadVa.reset(Hazel::VertexArray::Create());
	m_QuadBuffer.reset(Hazel::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));


	texture.reset(Hazel::Texture2D::Create("Assets/Textures/container2.png"));

	Hazel::BufferLayout layout2 =
	{ { "g_Position", Hazel::ShaderDataType::Float3 },
		{"g_Texture", Hazel::ShaderDataType::Float2}
	};

	m_QuadBuffer->SetLayout(layout2);
	this->quadVa->AddVertexBuffer(m_QuadBuffer);
	m_QuadIndex.reset(Hazel::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
	this->quadVa->SetIndexBuffer(m_QuadIndex);

	Hazel::ShaderLibray shaderLibrary;
	this->quadShader = shaderLibrary.Load("Assets/Shaders/quad.glsl");

	std::dynamic_pointer_cast<Hazel::OpenGLShader>(quadShader)->SetUniformInt("tex", 0);
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	this->m_CameraController.OnUpdate(ts);

	Hazel::Renderer::BeginScene(this->m_CameraController.GetCamera());

	this->quadShader->Bind();
	this->texture->Bind();
	Hazel::Renderer::Submit(this->quadVa, this->quadShader);

	Hazel::Renderer::EndScend();
}

void Sandbox2D::OnImGuiRender()
{

}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	this->m_CameraController.OnEvent(e);
}
