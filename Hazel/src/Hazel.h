#pragma once
#include <memory>

//for use by Hazel applications
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Core/Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"


//------Renderer----------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/OrthoGraphicCamera.h"
#include "Hazel/Renderer/FPSCamera.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"

//------------------------

#include "Hazel/Renderer/OrthoGraphicCameraController.h"
#include "Hazel/Renderer/FPSCameraController.h"

//------Entry point--------
//#include "Hazel/Core/EntryPoint.h"
//-------------------------
#include "Hazel/Debug/Instrumentor.h"