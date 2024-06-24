#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>
namespace Hazel
{
<<<<<<< HEAD
    static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
        }

        HZ_CORE_ASSERT(false, "Unknown ShaderDataType !");
        return 0;
    }


    OpenGLVertexArray::OpenGLVertexArray()
    {
        HZ_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &this->m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &this->m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(this->m_RendererID);
    }
    void OpenGLVertexArray::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }
    void OpenGLVertexArray::AddVertexBuffer(const Hazel::Ref<VertexBuffer>& vertexBuffer)
    {
        HZ_PROFILE_FUNCTION();

        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
        glBindVertexArray(this->m_RendererID);
        vertexBuffer->Bind();


        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (auto element : layout.GetElements())
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index++,
                element.GetComponentCount(),
                ShaderDataTypeToGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
        }
        this->m_VertexBuffers.push_back(vertexBuffer);
    }
    void OpenGLVertexArray::SetIndexBuffer(const Hazel::Ref<IndexBuffer>& indexBuffer)
    {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(this->m_RendererID);
        indexBuffer->Bind();

        this->m_IndexBuffers = indexBuffer;
    }
=======
	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType !");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &this->m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &this->m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(this->m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(this->m_RendererID);
		vertexBuffer->Bind();


		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout.GetElements())
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		this->m_VertexBuffers.push_back(vertexBuffer);

		/*for (auto element : layout.GetElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index++,
				element.GetComponentCount(),
				ShaderDataTypeToGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
		}*/
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(this->m_RendererID);
		indexBuffer->Bind();

		this->m_IndexBuffers = indexBuffer;
	}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}