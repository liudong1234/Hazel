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
		m_GizmoType(-1),
		m_HoveredEntity({})
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
		this->m_IconPlay = Texture2D::Create(std::string("Resources/Icons/play.png"));
		this->m_IconStop = Texture2D::Create(std::string("Resources/Icons/stop.png"));

        FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        spec.Width = 1280/*Application::Get().GetWindow().GetWidth()*/;
        spec.Height = 720/*Application::Get().GetWindow().GetHeight()*/;
        this->m_Framebuffer = Framebuffer::Create(spec);

        m_ActiveScene = CreateRef<Scene>();
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}
		//this->m_EditorCamera = EditorCamera(40.0f, 1.778f, 0.01f, 1000.0f);
		
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





        Renderer2D::ResetStatics();
        this->m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1, 1.0f });
        RenderCommand::Clear();

		//framebuffer color clear to -1
		this->m_Framebuffer->ClearAttachment(1, -1);

        //this->m_ActiveScene->OnUpdateRuntime(ts);
		
		switch (this->m_SceneState)
		{
			case  SceneState::Edit:
				if (this->m_ViewportFocus)
				{
					this->m_CameraController.OnUpdate(ts);
				}
				this->m_EditorCamera.OnUpdate(ts);
				this->m_ActiveScene->OnUpdateEditor(ts, this->m_EditorCamera);
				break;
			case SceneState::Play:
				this->m_ActiveScene->OnUpdateRuntime(ts);
				break;
			default:
				break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= this->m_ViewportBounds[0].x;
		my -= this->m_ViewportBounds[0].y;
		glm::vec2 viewportSize = this->m_ViewportBounds[1] - this->m_ViewportBounds[0];
		my = viewportSize.y - my;
		float mouseX = mx;
		float mouseY = my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = this->m_Framebuffer->ReadPixel(1, (int)mouseX, (int)mouseY);
			this->m_HoveredEntity = pixelData == -1 ? Entity{} : 
			Entity{ entt::entity(pixelData), this->m_ActiveScene.get() };
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

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveScene();
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
		this->m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Settings");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("QuadCalls:%d", stats.DrawCalls);
		ImGui::Text("QuadCount:%d", stats.QuadCounts);
		ImGui::Text("QuadIndexCount:%d", stats.GetQuadIndexCounts());
		ImGui::Text("QuadVertexCount:%d", stats.GetQuadVertexCounts());
		ImGui::Text("application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		std::string entityName = "None";
		if (this->m_HoveredEntity)
			entityName = this->m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("hoveredEntity:%s", entityName.c_str());
		
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
		//auto viewportOffset = ImGui::GetCursorPos();//包括的tab bar

		this->m_ViewportFocus = ImGui::IsWindowFocused();
		this->m_ViewportHover = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->SetBlockEvents(!this->m_ViewportFocus && !this->m_ViewportHover);
		ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();

		this->m_ViewportSize = { viewPanelSize.x, viewPanelSize.y };
		uint64_t textureID = m_Framebuffer->GetColorAttachmentID();

		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(this->m_ViewportSize.x, this->m_ViewportSize.y), { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content browser"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene((std::filesystem::path)path);
			}
			ImGui::EndDragDropTarget();
		}

		//鼠标位置
		auto viewWindowSize = ImGui::GetWindowSize();//viewport窗口大小
		//ImVec2 minBound = ImGui::GetWindowPos();
		//ImVec2 maxBound = { minBound.x + viewWindowSize.x, minBound.y + viewWindowSize.y };
		ImVec2 minBound = ImGui::GetWindowContentRegionMin();
		ImVec2 maxBound = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();//不包括包括的tab bar

		//minBound.x += viewportOffset.x;
		//minBound.y += viewportOffset.y;

		this->m_ViewportBounds[0] = { minBound.x + viewportOffset.x, minBound.y +viewportOffset.y };
		this->m_ViewportBounds[1] = { maxBound.x + viewportOffset.x, maxBound.y +viewportOffset.y };
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
		}

        ImGui::End();
        ImGui::PopStyleVar();

		UI_ToolBar();

        ImGui::End();
    }

	void EditorLayer::UI_ToolBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHoverd = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHoverd.x, buttonHoverd.y, buttonHoverd.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | 
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 8.0f;
		auto icon = m_SceneState == SceneState::Edit ? this->m_IconPlay : this->m_IconStop;
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x * 0.5f - size * 0.5f);
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { size, size }, ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();

		}
		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
	}

	void EditorLayer::OnScenePlay()
	{
		this->m_SceneState = SceneState::Play;
		this->m_ActiveScene = Scene::Copy(this->m_EditorScene);
		this->m_ActiveScene->OnUpdateStart();
	}

	void EditorLayer::OnSceneStop()
	{
		this->m_SceneState = SceneState::Edit;
		this->m_ActiveScene->OnUpdateStop();
		this->m_ActiveScene = this->m_EditorScene;
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selected = this->m_Panel.GetSelectedEntity();
		if (selected)
		{
			this->m_EditorScene->DuplicateEntity(selected);
		}
	}
	 
    void EditorLayer::OnEvent(Event& e)
    {
        this->m_CameraController.OnEvent(e);
		this->m_EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
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
				if (controlPress)
				{
					if (shiftPress)
						SaveAsScene();
					else
						SaveScene();
				}
				break;
			//scene command
			case HZ_KEY_D:
				if (controlPress)
					this->OnDuplicateEntity();
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

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressEvent& event)
	{
		if (event.GetMouseButton() == HZ_MOUSE_BUTTON_LEFT)
		{
			if (this->m_ViewportHover && !ImGuizmo::IsOver() && !Input::IsKeyPressed(HZ_KEY_LEFT_ALT))
			{
				this->m_Panel.SetSelectedEntity(this->m_HoveredEntity);
			}
		}
		return false;
	}


	void EditorLayer::NewScene()
	{
		this->m_ActiveScene = CreateRef<Scene>();
		this->m_ActiveScene->OnViewportResize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
		this->m_Panel.SetContext(this->m_ActiveScene);

		this->m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			OpenScene(std::filesystem::path(filepath));
		}
	}

	void EditorLayer::OpenScene(std::filesystem::path& path)
	{
		if (this->m_SceneState != SceneState::Edit)
			this->OnSceneStop();
		if (path.extension().string() != ".hazel")
		{
			HZ_WARN("Couldn't load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			this->m_EditorScene = newScene;
			this->m_EditorScene->OnViewportResize((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
			this->m_Panel.SetContext(this->m_EditorScene);
			this->m_ActiveScene = this->m_EditorScene;

			this->m_EditorScenePath = path;
		}
		else
			this->Dialog();

	}

	void EditorLayer::SaveAsScene()
	{
		std::string savepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!savepath.empty())
		{
			this->OnSerialzeScene(m_ActiveScene, savepath);

			this->m_EditorScenePath = savepath;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!this->m_EditorScenePath.empty())
			this->OnSerialzeScene(m_ActiveScene, this->m_EditorScenePath);
		else
			this->SaveAsScene();
	}

	void EditorLayer::OnSerialzeScene(Ref<Scene>& scene, std::filesystem::path path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::Dialog()
	{
		////if (ImGui::Button("Delete.."))
		//ImGui::OpenPopup("Delete?");

		ImGui::BeginPopupContextWindow("viewport");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		bool open = true;

		if (ImGui::BeginPopupModal("viewport", &open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
			ImGui::Separator();

			//static int unused_i = 0;
			//ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			//ImGui::EndPopup();
			ImGui::EndPopup();
			
		}
	}


}
