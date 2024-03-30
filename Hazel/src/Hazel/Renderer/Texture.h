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
    };

    class Texture2D :
        public Texture
    {
    public:
        static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);

        static Ref<Texture2D> Create(std::string& filepath);

    };

}
