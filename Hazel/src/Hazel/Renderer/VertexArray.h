#pragma once
#include "Hazel/Renderer/Buffer.h"

#include <memory>


namespace Hazel
{
<<<<<<< HEAD
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

        static Ref<VertexArray> Create();

    private:

    };
=======
	class VertexArray
	{
	public:
		virtual ~VertexArray() {};
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffers() const = 0;

		static Ref<VertexArray> Create();

	private:

	};
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
}