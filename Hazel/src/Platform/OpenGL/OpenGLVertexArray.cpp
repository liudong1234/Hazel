﻿#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>
namespace Hazel
{
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
}