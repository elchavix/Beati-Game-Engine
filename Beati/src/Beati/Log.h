#pragma once


#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Beati
{
	class BEATI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

// Core log macros
#define BE_CORE_TRACE(...)   ::Beati::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BE_CORE_INFO(...)    ::Beati::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BE_CORE_WARN(...)    ::Beati::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BE_CORE_ERROR(...)   ::Beati::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BE_CORE_FATAL(...)   ::Beati::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BE_CLIENT_TRACE(...)   ::Beati::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BE_CLIENT_INFO(...)    ::Beati::Log::GetClientLogger()->info(__VA_ARGS__)
#define BE_CLIENT_WARN(...)    ::Beati::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BE_CLIENT_ERROR(...)   ::Beati::Log::GetClientLogger()->error(__VA_ARGS__)
#define BE_CLIENT_FATAL(...)   ::Beati::Log::GetClientLogger()->fatal(__VA_ARGS__)