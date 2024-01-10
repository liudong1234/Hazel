#pragma once
#include <memory>

//for use by Hazel applications
#include "Hazel/Application.h"
#include "Hazel/Layer.h"
#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/MouseButtonCodes.h"
#include "Hazel/Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"


//------Renderer----------
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/RenderCommand.h"

#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/OrthoGraphicCamera.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"

//------------------------

//------Entry point--------
#include "Hazel/EntryPoint.h"
//-------------------------