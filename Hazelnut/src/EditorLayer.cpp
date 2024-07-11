#include "hzpch.h"
#include "EditorLayer.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Debug/Instrumentor.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Scene/SceneSerializer.h"

#include <chrono>
#include <glm/gtc/type_ptr.hpp >

#include "Hazel/Utils/PlatformUtils.h"
#include "ImGuizmo.h" 
#include "Hazel/Math/Math.h"


namespace Hazel
{
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"),
        m_CameraController(1280.0f / 720.0f),
        m_ViewportSize(glm::vec2(0.0f)),
        m_ViewportFocus(false),
        m_ViewportHover(false),
		m_GizmoType(-1)
    {
		m_EditorCamera = EditorCamera();
    }

    EditorLayer::~EditorLayer()
    {
        Renderer2D::Shutdown();
    }

    void EditorLayer::OnAttach()
    {
        HZ_PROFILE_FUNCTION();
        this->quadTexture = Texture2D::Create(std::string("Assets/map/spritesheet/roguelikeSheet_magenta.png"));


        FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        spec.Width = 1280/*Application::Get().GetWindow().GetWidth()*/;
        spec.Height = 720/*Application::Get().GetWindow().GetHeight()*/;
        this->m_Framebuffer = Framebuffer::Create(spec);

        m_ActiveScene = CreateRef<Scene>();

		#if 0
        auto square = m_ActiveScene->CreateEntity("Red Square");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
        
        auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
        greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

        this->m_SquareEntity = square;
        
        
        this->m_CameraEntity = this->m_ActiveScene->CreateEntity("Camera");
        this->m_CameraEntity.AddComponent<CameraComponent>();

        this->m_SecondCameraEntity = this->m_ActiveScene->CreateEntity("Clip-Space Camera");
        auto& second = this->m_SecondCameraEntity.AddComponent<CameraComponent>();
        second.Primary = false;

        class CameraColltroller : public ScriptableEntity
        {
        public:
            void OnCreate() 
            {
                /*auto& transform = GetComponent<TransformComponent>().Translation;
                transform.x = rand() % 10 - 5.0f;*/
            }
            void OnDestroy() {}
            void OnUpdate(TimeStep ts) 
            {
                //HZ_INFO("OnUpdate:{0}", ts);
                auto& translation = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;
                if (Input::IsKeyPressed(HZ_KEY_A))
                    translation.x -= ts * speed;                
                if (Input::IsKeyPressed(HZ_KEY_D))
                    translation.x += ts * speed;                
                if (Input::IsKeyPressed(HZ_KEY_W))
                    translation.y += ts * speed;                
                if (Input::IsKeyPressed(HZ_KEY_S))
                    translation.y -= ts * speed;
            }
        
        };
        
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraColltroller>();
        m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraColltroller>();
        
		#endif
        this->m_Panel.SetContext(this->m_ActiveScene);  
    }

    void EditorLayer::OnDetach()
    {
        HZ_PROFILE_FUNCTION();

    }

    void EditorLayer::OnUpdate(TimeStep ts)
    {
        //InstrumentationTimer timer("EditorLayer Onupdate", [&](ProfileResult result) {this->m_ProfileResults.push_back(result); });
        HZ_PROFILE_FUNCTION();

        if (FramebufferSpecification spec = this->m_Framebuffer->GetSpecification();
            this->m_ViewportSize.x > 0.0f && this->m_ViewportSize.y > 0.0f && //
            (spec.Width != this->m_ViewportSize.x || spec.Height != this->m_ViewportSize.y))
        {
            this->m_Framebuffer->Resize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
            this->m_CameraController.OnResize(this->m_ViewportSize.x, this->m_ViewportSize.y);
			this->m_EditorCamera.SetViewportSize(this->m_ViewportSize.x, this->m_ViewportSize.y);
            this->m_ActiveScene->OnViewportResize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);

        }

		if (this->m_ViewportFocus)
		{
            this->m_CameraController.OnUpdate(ts);
		}
		this->m_EditorCamera.OnUpdate(ts);


        Renderer2D::ResetStatics();
        this->m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0, 1.0f });
        RenderCommand::Clear();

        //this->m_ActiveScene->OnUpdateRuntime(ts);
        this->m_ActiveScene->OnUpdateEditor(ts, this->m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= this->m_ViewportBounds[0].x;
		my -= this->m_ViewportBounds[0].y;
		glm::vec2 viewportSize = this->m_ViewportBounds[1] - this->m_ViewportBounds[0];
		my = viewportSize.y - my;
		float mouseX = (int)mx;
		float mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && (int)viewportSize.y)
		{
			HZ_CORE_WARN("{0}", this->m_Framebuffer->ReadPixel(1, mouseX, mouseY));
			HZ_CORE_INFO("MOUSE: {0}, {1}", mouseX, mouseY);
		}


        this->m_Framebuffer->UnBind();
    }

    void EditorLayer::OnImGuiRender()
    {
        HZ_PROFILE_FUNCTION();

        static bool p_open = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        ImGui::PopStyleVar();


        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{
					SaveAsScene();
				}
				if (ImGui::MenuItem("Exit"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

        this->m_Panel.OnImGuiRender();

        ImGui::Begin("Settings");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("QuadCalls:%d", stats.DrawCalls);
        ImGui::Text("QuadCount:%d", stats.QuadCounts);
        ImGui::Text("QuadIndexCount:%d", stats.GetQuadIndexCounts());
        ImGui::Text("QuadVertexCount:%d", stats.GetQuadVertexCounts());
        ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        /*ImGui::Separator();
        auto& tag = this->m_SquareEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("%s", tag.c_str());

        auto& sprite = this->m_SquareEntity.GetComponent<SpriteRendererComponent>();
        ImGui::ColorEdit4("Background", &sprite.Color[0]);
        ImGui::Separator();
        ImGui::DragFloat3("camera transform", glm::value_ptr(
            this->m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

        if (ImGui::Checkbox("Camera A", &this->m_PrimaryCamera))
        {
            //this->m_CameraEntity.GetComponent<CameraComponent>().Primary = this->m_PrimaryCamera;
            //this->m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !this->m_PrimaryCamera;
        }

        {
            auto& camera = this->m_SecondCameraEntity.GetComponent<CameraComponent>().Camera;
            float size = camera.GetOrthographicSize();
            if (ImGui::DragFloat("secondCamera ortho", &size))
                camera.SetOrthographicSize(size);
        }*/
        ImGui::End();


        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

        ImGui::Begin("viewport");
		auto viewportOffset = ImGui::GetCursorPos();//包括的tab bar

        this->m_ViewportFocus = ImGui::IsWindowFocused();
        this->m_ViewportHover = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->SetBlockEvents(!this->m_ViewportFocus && !this->m_ViewportHover);
        ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();
            
        this->m_ViewportSize = { viewPanelSize.x, viewPanelSize.y };
		uint64_t textureID = m_Framebuffer->GetColorAttachmentID();

        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(this->m_ViewportSize.x, this->m_ViewportSize.y), { 0, 1 }, { 1, 0 });

		//鼠标位置
		auto viewWindowSize = ImGui::GetWindowSize();//viewport窗口大小
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;
		ImVec2 maxBound = { minBound.x + viewWindowSize.x, minBound.y + viewWindowSize.y };

		this->m_ViewportBounds[0] = { minBound.x, minBound.y };
		this->m_ViewportBounds[1] = { maxBound.x, maxBound.y };
		//HZ_CORE_INFO("minBound: {0}, {1}", minBound.x, minBound.y);

		//gizmos
		auto selectedEntity = this->m_Panel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowW = ImGui::GetWindowWidth();
			float windowH = ImGui::GetWindowHeight();

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowW, windowH);
			//runtime camera
			/*auto cameraEntity = this->m_ActiveScene->GetPrimaryCamera();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			//editor camera
			const glm::mat4& cameraProjection = this->m_EditorCamera
				.GetProjection();
			glm::mat4 cameraView = this->m_EditorCamera.GetViewMatrix();

			//entity
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4& transform = tc.GetTransform();
			
			//snapping 捕捉按住某键可以有一定度数
			bool snap = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (this->m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };
			
			
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				ImGuizmo::OPERATION(m_GizmoType), ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}

			//snapping
			//bool snap = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);


		}

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::End();
    }
	 
    void EditorLayer::OnEvent(Event& e)
    {
        this->m_CameraController.OnEvent(e);
		this->m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }
	
	bool EditorLayer::OnKeyPressed(KeyPressEvent& event)
	{
		if (event.GetRepeatCount() > 0)
			return false;
		bool controlPress = Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(HZ_KEY_RIGHT_CONTROL);
		bool shiftPress = Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(HZ_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
			case HZ_KEY_N:
				if (controlPress)
					NewScene();
				break;
			case HZ_KEY_O:
				if (controlPress)
					OpenScene();
				break;
			case HZ_KEY_S:
				if (controlPress && shiftPress)
					SaveAsScene();
				break;

			//GIZMOS
			case HZ_KEY_F1:
				this->m_GizmoType = -1;
				break;
			case HZ_KEY_F2:
				this->m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case HZ_KEY_F3:
				this->m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case HZ_KEY_F4:
				this->m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			default:
				break;
		}

		return true;
	}

	void EditorLayer::NewScene()
	{
		this->m_ActiveScene = CreateRef<Scene>();
		this->m_ActiveScene->OnViewportResize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
		this->m_Panel.SetContext(this->m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			this->m_ActiveScene = CreateRef<Scene>();
			this->m_ActiveScene->OnViewportResize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
			this->m_Panel.SetContext(this->m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveAsScene()
	{
		std::string savepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!savepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(savepath);
		}
	}
}
