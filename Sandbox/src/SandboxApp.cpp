#include "Hazel.h"

#include <string>
#include "Hazel/Core/TimeStep.h"
//#include "Model.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLShader.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

class ExampleLayer :
	public Hazel::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_Camera(-1.0f, 1.0f, -1.0f, 1.0f),
		cameraPosition(0.0f),
		cameraRotation(0.0f)
	{
		this->m_VertexArray.reset(Hazel::VertexArray::Create());
		float vertices[] =
		{
			-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout =
		{
			{ "g_Position", Hazel::ShaderDataType::Float3 },
			{ "g_Color", Hazel::ShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(layout);
		this->m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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
		this->shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));


		//正方形

		float quadVertices[] =
		{
			//position		
			-0.7f, -0.7f, 0.0f, 0.0f, 0.0f,
			 0.7f, -0.7f, 0.0f, 1.0f, 0.0f,
			 0.7f,  0.7f, 0.0f, 1.0f, 1.0f,
			-0.7f,  0.7f, 0.0f, 0.0f, 1.0f
		};
		uint32_t quadIndices[] = { 0, 1, 2, 0, 2, 3 };
		this->quadVa.reset(Hazel::VertexArray::Create());
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));


		texture.reset(Hazel::Texture2D::Create("Assets/Textures/container2.png"));

		Hazel::BufferLayout layout2 = 
		{ { "g_Position", Hazel::ShaderDataType::Float3 },
			{"g_Texture", Hazel::ShaderDataType::Float2}
		};

		m_VertexBuffer->SetLayout(layout2);
		this->quadVa->AddVertexBuffer(m_VertexBuffer);
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
		this->quadVa->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc2 =
			R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			layout(location = 1) in vec2 aTexCoords;
			out vec3 v_Position;
			out vec2 v_TexCoord;
			uniform mat4 projection;
			uniform mat4 view;

			void main()
			{
				v_Position = a_Position;
				v_TexCoord = aTexCoords;
				gl_Position = projection * view * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc2 =
			R"(
			#version 330 core
			out vec4 FragColor;
			uniform sampler2D tex;
			in vec2 v_TexCoord;
			void main()
			{
				FragColor = texture(tex, v_TexCoord);
			}
		)";


		//this->quadShader.reset(Hazel::Shader::Create(vertexSrc2, fragmentSrc2));
		this->quadShader.reset(Hazel::Shader::Create((std::string)"Assets/Shaders/quad.glsl"));

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(quadShader)->SetUniformInt("tex", 0);
	}

	void OnUpdate(Hazel::TimeStep ts) override
	{
		Hazel::RenderCommand::Clear();
		Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});

		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
			this->cameraRotation += this->rotationSpeed * ts;
		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
			this->cameraRotation -= this->rotationSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			this->cameraPosition.y += this->moveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			this->cameraPosition.y -= this->moveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			this->cameraPosition.x += this->moveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			this->cameraPosition.x -= this->moveSpeed * ts;

		this->m_Camera.SetRotation(this->cameraRotation);
		this->m_Camera.SetPosition(this->cameraPosition);

		Hazel::Renderer::BeginScene(this->m_Camera);
		this->quadShader->Bind();
		this->texture->Bind();
		Hazel::Renderer::Submit(this->quadVa, this->quadShader);
		Hazel::Renderer::Submit(this->m_VertexArray, this->shader);
		Hazel::Renderer::EndScend();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Hazel::Event& e) override
	{
		/*Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::KeyPressEvent>(BIND_EVENT_FN(ExampleLayer::OnKeyBoardDown));*/
	}

	bool OnKeyBoardDown(Hazel::Event& event)
	{
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressEvent& e = (Hazel::KeyPressEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_UP)
				this->cameraPosition.y += this->moveSpeed;
			if (e.GetKeyCode() == HZ_KEY_DOWN)
				this->cameraPosition.y -= this->moveSpeed;
			if (e.GetKeyCode() == HZ_KEY_RIGHT)
				this->cameraPosition.x += this->moveSpeed;
			if (e.GetKeyCode() == HZ_KEY_LEFT)
				this->cameraPosition.x -= this->moveSpeed;
			return true;
		}

		return false;
	}

private:
	//unsigned int m_VertexArray;//三角形
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::Shader> shader;

	Hazel::Ref<Hazel::VertexArray> quadVa;
	Hazel::Ref<Hazel::Shader> quadShader;


	Hazel::Ref<Hazel::Texture> texture;

	Hazel::OrthoGraphicCamera m_Camera;

	glm::vec3 cameraPosition;
	float cameraRotation;
	float moveSpeed = 1.0f;
	float rotationSpeed = 30.0f;
};

class Sandbox :
	public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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