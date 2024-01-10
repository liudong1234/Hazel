#pragma once
#include "Hazel/Renderer/Buffer.h"

#include <memory>


namespace Hazel
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Hazel::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Hazel::Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Hazel::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Hazel::Ref<IndexBuffer>& GetIndexBuffers() const = 0;

		static VertexArray* Create();

	private:

	};
}