#include "hzpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel
{
    //----------------------OpenGLVertexBuffer--------------------------///
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &this->m_RendererID);
        this->Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteBuffers(1, &this->m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);
    }

    void OpenGLVertexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    //------------------------OpenGLIndexBuffer------------------------///
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        :m_Count(count)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &this->m_RendererID);
        this->Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteBuffers(1, &this->m_RendererID);
    }

    uint32_t OpenGLIndexBuffer::GetCount() const
    {
        return this->m_Count;
    }

    void OpenGLIndexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
    }

    void OpenGLIndexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}
