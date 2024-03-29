#include "hzpch.h"
#include "Sandbox2D.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

template<typename Fn>
class InstrumentationTimer
{
private:
	const char* name;//方法名
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	bool m_stop;
	Fn m_Func;
public:
	InstrumentationTimer(const char* name, Fn&& func) :
		m_Func(func),
		name(name),
		m_stop(false)
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	void stop()
	{
		auto m_end = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(m_end).time_since_epoch().count();

		//uint32_t threadId = std::hash<std::thread::id>()(std::this_thread::get_id());

		float during = (end - start) * 0.001f;

		this->m_Func({ this->name, during });
		//std::cout << name << ": " << (end - start) * 0.001 << "ms\n";
		m_stop = true;
	}

	~InstrumentationTimer()
	{
		if (!m_stop)
			stop();
	}
};

#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name, [&](ProfileResult result){m_ProfileResults.push_back(result);})

Sandbox2D::Sandbox2D() :
<<<<<<< HEAD
	Layer("Sandbox2D"),
	m_CameraController(1280.0f / 720.0f),
	m_Pos(glm::vec3(1.0f)),
	m_Color(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)),
	m_QuadPos(glm::vec2(1.0f)),
	m_QuadSize(glm::vec2(1.0f)),
	m_QuadAngle(0.0f)
=======
    Layer("Sandbox2D"),
    m_CameraController(1280.0f / 720.0f),
    m_Pos(glm::vec3(1.0f)),
    m_Color(glm::vec4(0.3f, 0.4f, 0.5f, 1.0f)),
    m_QuadPos(glm::vec2(0.0f)),
    m_QuadSize(glm::vec2(1.0f)),
    m_QuadAngle(0.0f)
>>>>>>> 100debe (Improving 2D Rendering)
{

}

Sandbox2D::~Sandbox2D()
{
	Hazel::Renderer2D::Shutdown();
}

void Sandbox2D::OnAttach()
{

	this->quadTexture = Hazel::Texture2D::Create(std::string("Assets/Textures/1.png"));

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	//InstrumentationTimer timer("sandbox2D Onupdate", [&](ProfileResult result) {this->m_ProfileResults.push_back(result); });
	PROFILE_SCOPE("Sandbox2D OnUpdate");
	this->m_CameraController.OnUpdate(ts);
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.8f, 0.2f, 0.4f, 1.0f });

<<<<<<< HEAD
	Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad(this->m_QuadPos, this->m_QuadSize, this->m_Color, this->m_QuadAngle);
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, this->quadTexture);
=======
    {
        HZ_PROFILE_SCOPE("Renderer Prop");
        Hazel::RenderCommand::Clear();
        Hazel::RenderCommand::SetClearColor({ 0.8f, 0.2f, 0.4f, 1.0f });
    }

    {
        HZ_PROFILE_SCOPE("Draw Quad");
        Hazel::Renderer2D::BeginScene(this->m_CameraController.GetCamera());
        Hazel::Renderer2D::DrawQuad({ -0.5f, -0.7f }, { 1.0f, 1.0f }, { 0.1f, 0.2f, 0.3f, 1.0f });
        Hazel::Renderer2D::DrawRotateQuad({ 1.0f, 0.8f }, this->m_QuadSize, this->m_QuadAngle, this->m_Color);
        Hazel::Renderer2D::DrawRotateQuad(this->m_QuadPos, this->m_QuadSize, this->m_QuadAngle, this->quadTexture, 2.0f, {1.0f, 0.8f, 0.8f, 1.0f});
        Hazel::Renderer2D::EndScene();
    }
>>>>>>> 100debe (Improving 2D Rendering)

	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::DragFloat3(u8"位置", &this->m_Pos.x, 0.1f);
	ImGui::DragFloat2("QuadPos", &this->m_QuadPos[0], 0.1f);
	ImGui::DragFloat2("QuadSize", &this->m_QuadSize[0], 0.1f);
	ImGui::DragFloat("QuadRotation", &this->m_QuadAngle, 5.0f);
	ImGui::ColorEdit4("Background", &this->m_Color[0]);

	for (auto& result : this->m_ProfileResults)
	{
		char txt[63];
		strcpy(txt, "%.3fms  ");
		strcat(txt, result.Name);
		ImGui::Text(txt, result.Time);
	}
	this->m_ProfileResults.clear();
	ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	this->m_CameraController.OnEvent(e);
}
