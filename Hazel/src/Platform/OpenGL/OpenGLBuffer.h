#pragma once
#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
    class OpenGLVertexBuffer
        : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        OpenGLVertexBuffer(uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;
        virtual const BufferLayout& GetLayout() const override { return this->m_Layout; };
        virtual void SetLayout(const BufferLayout& layout) override { this->m_Layout = layout; }
        virtual void SetData(const void* data, uint32_t size) override;
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer
        : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual uint32_t GetCount() const override;
        virtual void Bind() const override;
        virtual void UnBind() const override;

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

}