﻿#include "Hazel.h"

#include <string>

#include"imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Hazel/Core/TimeStep.h"

class ExampleLayer :
	public Hazel::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_Camera(-1.0f, 1.0f, -1.0f, 1.0f),
		m_CameraPos(0.0f),
		m_CameraRotation(0.0f),
		m_RotationSpeed(10.0f),
		m_Pos(glm::vec3(1.0f))
	{
		this->m_VertexArray.reset(Hazel::VertexArray::Create());
		float vertices[] =
		{
			-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout =
		{
			{ "g_Position", Hazel::ShaderDataType::Float3 },
			{ "g_Color", Hazel::ShaderDataType::Float4 }
		};
		m_VertexBuffer->SetLayout(layout);
		this->m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;
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
		this->shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		float quadVertices[] =
		{
			-0.5f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f,  -0.5f, 0.0f,
			0.5f,   0.5f, 0.0f
		};
		this->quadVa.reset(Hazel::VertexArray::Create());
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(quadVertices, sizeof(quadVertices)));

		layout = { { "g_Position", Hazel::ShaderDataType::Float3 } };
		m_VertexBuffer->SetLayout(layout);
		this->quadVa->AddVertexBuffer(m_VertexBuffer);

		uint32_t quadIndices[] = { 0, 1, 2, 0, 3, 2 };
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t)));
		this->quadVa->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc2 =
			R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			uniform mat4 projection;
			uniform mat4 view;
			uniform mat4 transform;
			void main()
			{
				gl_Position = projection * view * transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc2 =
			R"(
			#version 330 core
			layout(location=0) out vec4 color;
			uniform vec4 Color;
			void main()
			{
				color = Color;
			}
		)";
		this->quadShader.reset(new Hazel::Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Hazel::TimeStep ts) override
	{
		HZ_INFO("time : {0}, {1}", ts.GetSeconds(), ts.GetMilliSeconds());

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			this->m_CameraPos.y += this->m_MoveSpeed * ts.GetSeconds();
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			this->m_CameraPos.y -= this->m_MoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			this->m_CameraPos.x -= this->m_MoveSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			this->m_CameraPos.x += this->m_MoveSpeed * ts;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->m_Pos);

		Hazel::RenderCommand::SetClearColor({ 1.0f, 0.1f, 1.0f, 1.0f });
		Hazel::RenderCommand::Clear();

		this->m_Camera.SetPosition(this->m_CameraPos);
		//this->m_Camera.SetRotation(180.0f);

		Hazel::Renderer::BeginScene(this->m_Camera);

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
					this->quadShader->SetUniformFloat4("Color", red);
				else
					this->quadShader->SetUniformFloat4("Color", blue);
				Hazel::Renderer::Submit(this->quadVa, this->quadShader, transform);

			}
		}
		Hazel::Renderer::Submit(this->m_VertexArray, this->shader);
		Hazel::Renderer::EndScend();

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
		ImGui::End();
	}

	void OnEvent(Hazel::Event& e) override
	{
		//HZ_TRACE("{0}", e);
	}
private:
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> shader;

	std::shared_ptr<Hazel::VertexArray> quadVa;
	std::shared_ptr<Hazel::Shader> quadShader;
	Hazel::OrthographicCamera m_Camera;

	float m_MoveSpeed = 1.0f;
	float m_RotationSpeed;

	glm::vec3 m_CameraPos;
	float m_CameraRotation;

	glm::vec3 m_Pos;
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