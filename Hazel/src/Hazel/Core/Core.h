#pragma once

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
		return std::make_unique<T>(std::forward<Args>(args)...)
	}
	/*
	  这个函数是一个模板函数，它接受任意数量的参数，并使用这些参数构造类型 T 的对象，然后返回一个 Scope<T> 智能指针。函数内部使用了 std::make_unique，这是C++14引入的标准库函数，用于创建一个 std::unique_ptr 对象。
		typename T: 模板参数，表示要分配内存的对象类型。
		typename ... Args: 模板参数包，表示构造对象时所需的任意数量的参数。
		Scope<T>: 返回类型，是一个 std::unique_ptr 实例，用于管理 T 类型的对象。
		constexpr: 表示这个函数在编译时会产生常量表达式。
	*/

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}