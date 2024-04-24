#pragma once

//for use by Hazel applications
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Core/Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"

//----------Renderer-------------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Framebuffer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Renderer/SubTexture2D.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/VertexArray.h"

//---------Scene----------------
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/ScriptableEntity.h"
#include "Hazel/Scene/Components.h"
//----------End ---------------------

#include "Hazel/Renderer/OrthographicCameraController.h"

//------Entry point--------
//#include "Hazel/Core/EntryPoint.h"
//-------------------------
#include "Hazel/Debug/Instrumentor.h"