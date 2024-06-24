<<<<<<< HEAD
﻿#pragma once

#include <string>

namespace Hazel
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) = 0;
        virtual void UnBind() = 0;
        virtual bool operator == (const Texture& other) const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual uint32_t GetRendererID() = 0;
    };

    class Texture2D :
        public Texture
    {
    public:
        static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);

        static Ref<Texture2D> Create(std::string& filepath);

    };

}
=======
#pragma once


namespace Hazel
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void UnBind() = 0;
		virtual std::string GetPath() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual bool operator == (const Texture& other) = 0;
	};

	class Texture2D :
		public Texture
	{
	public:
		static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
		static Ref<Texture2D> Create(std::string filepath);
	};

}
>>>>>>> 105b83b5a7f24a67807d82b5cf407cf934ec75b7
