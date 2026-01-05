#pragma once

#include "Beati/Core/PlatformDetection.h"

#include <memory>

#ifdef BE_DEBUG
#if defined(BE_PLATFORM_WINDOWS)
#define BE_DEBUGBREAK() __debugbreak()
#elif defined(BE_PLATFORM_LINUX)
#include <signal.h>
#define BE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define BE_ENABLE_ASSERTS
#else
#define BE_DEBUGBREAK()
#endif

#define BE_EXPAND_MACRO(x) x
#define BE_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define BE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Beati {

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

#include "Beati/Core/Log.h"
#include "Beati/Core/Assert.h"
