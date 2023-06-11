#pragma once
#pragma execution_character_set("utf-8")
#include <memory>
#include <iostream>
#include <cinttypes>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define MC_PLATFORM_WINDOWS
    #ifdef _WIN64
        #define MC_PLATFORM_WINDOWS_X64
    #else
        #define MC_PLATFORM_WINDOWS_X64_86
    #endif

    #define MC_USE_GPU_ACCELERATION
#elif __APPLE__
    #define MC_PLATFORM_MACOS
#elif __linux__
    #define MC_PLATFORM_LINUX
#elif __unix__ // all unixes not caught above
    #define MC_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
    #define MC_PLATFORM_POSIX
#else
    #error "Unknown compiler"
#endif

#include "StackTracer.h"

//
// Uncomment this line in order to receive
// a full frame stacktrace on a failed assert call.
//
#define MC_DEBUG_ENABLE_STACKTRACE

#ifdef MC_DEBUG_ENABLE_STACKTRACE
    #define PrintStackTrace() _printDebugStackTrace();
#else
    #define PrintStackTrace()
#endif

#ifndef NDEBUG
#define CORE_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                    << " line " << __LINE__ << ": " << message << std::endl; \
        PrintStackTrace() \
        std::terminate(); \
    }
#else
#define CORE_ASSERT(condition, message)
#endif

namespace mc {
    template <typename T>
    using Shared = std::shared_ptr<T>;

    template <typename T>
    using Unique = std::unique_ptr<T>;

    template <typename T>
    using Weak = std::weak_ptr<T>;

    template <typename T, typename... Args>
    constexpr Shared<T> MakeRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    constexpr Unique<T> MakeUniqueRef(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
} // namespace mc
