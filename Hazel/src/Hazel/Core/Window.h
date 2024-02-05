#pragma once

#include "hzpch.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Height;
        unsigned int Width;

        WindowProps(const std::string title = "Hazel Engine", unsigned int width = 1280, unsigned int height = 720) :
            Title(title), Height(height), Width(width)
        {
        }
    };

    class HAZEL_API Window
    {
    public:
        //事件回调
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}
        virtual void OnUpdate() = 0;
        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void* GetNativeWindow() const = 0;

        //window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;//设垂直同步
        virtual bool IsVSync() const = 0;

        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}