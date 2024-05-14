#pragma once
#include <memory>

#ifdef _WIN32
//window x86/x64
    #ifdef _WIN64
        #define HZ_PLATFORM_WINDOWS
    #else
        #error "x86 builds aren't supported!"
    #endif
#elif defined(__APPLE__) || define(__MACH)
    #include <TargetConditionals.h>
   
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define HZ_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define HZ_PLATFORM_MAC == 1
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif
#elif define(__ANDROID__)
    #define HZ_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif #define(__linux__)
    #define HZ_PLATFORM_LINUX
#error "linux is not supported!"
#endif


#ifdef HZ_PLATFORM_WINDOW
#if HZ_SHARED
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#else
#define HAZEL_API 
#endif
#else
#error Hazel only supports Windows!
#endif // HZ_PLATFORM_WINDOW

#ifdef HZ_DEBUG
#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x, ...) {if(!(x)) {HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define HZ_CORE_ASSERT(x, ...) {if (!(x)) {HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();} }
#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Hazel
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}