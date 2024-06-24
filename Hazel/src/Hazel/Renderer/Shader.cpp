#include "hzpch.h"
<<<<<<< HEAD
#include "Renderer.h"

#include "Shader.h"
=======
#include "Shader.h"
#include "Renderer.h"
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
<<<<<<< HEAD
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(filepath);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Shader");
        return nullptr;
    }
    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
            default:
                break;
        }
        HZ_CORE_ASSERT(false, "Unknown Shader");
        return nullptr;
    }
    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto name = shader->GetName();
        HZ_CORE_ASSERT(this->m_Shaders.find(name) != this->m_Shaders.end(), "shader already exist!");
        this->m_Shaders[name] = shader;
    }
    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        HZ_CORE_ASSERT(!this->Exist(name), "shader already exist!");
        this->m_Shaders[name] = shader;
    }
    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        this->Add(shader);
        return shader;
    }
    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        this->Add(name, shader);
        return shader;
    }
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        HZ_CORE_ASSERT(this->Exist(name), "shader not exist!");
        return this->m_Shaders[name];
    }
    bool ShaderLibrary::Exist(const std::string& name) const
    {
        if (this->m_Shaders.find(name) == this->m_Shaders.end())
            return false;
        return true;
    }
=======
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI None is currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		default:
			break;
		}
		HZ_CORE_ASSERT(false, "Unknown Renderer");
		return nullptr;

	}
	
	//--------------------------ShaderLibrary---------------------------------
	void ShaderLibray::Add(const std::string& name, const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!this->Exist(name), "Shader alreadly exist");//当!this->Exist(name)为false时，中断，即this->Exist(name) == true时
		this->m_Shaders[name] = shader;
	}

	void ShaderLibray::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		this->Add(name, shader);
	}

	Ref<Shader> ShaderLibray::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		this->Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibray::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		this->Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibray::Get(const std::string& name)
	{
		HZ_CORE_ASSERT(Exist(name), "Shader not exist");
		return this->m_Shaders[name];
	}

	bool ShaderLibray::Exist(const std::string& name) const
	{
		return this->m_Shaders.find(name) != this->m_Shaders.end();
	}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}
