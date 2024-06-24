#pragma once
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
	class OpenGLVertexArray :
		public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void UnBind() const override;

<<<<<<< HEAD
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return this->m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffers() const override { return this->m_IndexBuffers; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;
=======
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return this->m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffers() const override { return this->m_IndexBuffers; }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffers;
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
	};

}


