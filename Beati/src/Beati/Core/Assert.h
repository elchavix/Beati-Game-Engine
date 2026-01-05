#pragma once

#include "Beati/Core/Base.h"
#include "Beati/Core/Log.h"
#include <filesystem>

#ifdef BE_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define BE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { BE##type##ERROR(msg, __VA_ARGS__); BE_DEBUGBREAK(); } }
#define BE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) BE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define BE_INTERNAL_ASSERT_NO_MSG(type, check) BE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", BE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define BE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define BE_INTERNAL_ASSERT_GET_MACRO(...) BE_EXPAND_MACRO( BE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, BE_INTERNAL_ASSERT_WITH_MSG, BE_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define BE_ASSERT(...) BE_EXPAND_MACRO( BE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define BE_CORE_ASSERT(...) BE_EXPAND_MACRO( BE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define BE_ASSERT(...)
#define BE_CORE_ASSERT(...)
#endif
