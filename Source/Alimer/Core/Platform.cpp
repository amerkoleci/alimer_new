// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/Platform.h"
#if defined(_WIN32)
#include "Alimer/Platform/Win32/WindowsPlatform.h"
#include <array>
#elif defined(__EMSCRIPTEN__)
#include "Alimer/Core/Log.h"
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif
#include <thread>

namespace Alimer
{
    PlatformID GetPlatformID()
    {
#if ALIMER_PLATFORM_WINDOWS
        return PlatformID::Windows;
#elif ALIMER_PLATFORM_XBOX_ONE
        return PlatformID::XboxOne;
#elif ALIMER_PLATFORM_XBOX_SCARLETT
        return PlatformID::XboxScarlett;
#elif ALIMER_PLATFORM_LINUX
        return PlatformID::Linux;
#elif ALIMER_PLATFORM_ANDROID
        return PlatformID::Android;
#elif ALIMER_PLATFORM_MACOS
        return PlatformID::MacOS;
#elif ALIMER_PLATFORM_IOS
        return PlatformID::iOS;
#elif ALIMER_PLATFORM_BROWSER
        return PlatformID::Browser;
#else
#   error "Unknown platform"
#endif
    }

    PlatformFamily GetPlatformFamily()
    {
#if ALIMER_PLATFORM_FAMILY_MOBILE
        return PlatformFamily::Mobile;
#elif ALIMER_PLATFORM_FAMILY_DESKTOP
        return PlatformFamily::Desktop;
#elif ALIMER_PLATFORM_FAMILY_CONSOLE
        return PlatformFamily::Console;
#else
        return PlatformFamily::Unknown;
#endif
    }

    uint32_t GetNumPhysicalCPUs()
    {
        uint32_t cpuCount = 0;
#if defined(__EMSCRIPTEN__)
        cpuCount = std::thread::hardware_concurrency();
        return cpuCount;
#elif defined(_WIN32)
        DWORD bufferSize = 0;
        GetLogicalProcessorInformation(nullptr, &bufferSize);
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            const size_t entryCount = bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
            std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> entries(entryCount);
            if (GetLogicalProcessorInformation(entries.data(), &bufferSize))
            {
                for (const auto& entry : entries)
                {
                    if (entry.Relationship == RelationProcessorCore)
                        cpuCount++;
                }
            }
        }
#endif

        // This is as good as it gets on remaining platforms.
        if (cpuCount == 0)
            cpuCount = std::thread::hardware_concurrency();

        return Max(1u, cpuCount);
    }

    /* Dynamic Library */
    LibraryHandle OpenLibrary(const char* path) noexcept
    {
#if defined(_WIN32)
        // Use SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS to avoid DLL search path attacks.
        const bool is_absolute = path != nullptr && ((path[0] != '\0' && path[1] == ':') || (path[0] == '\\' && path[1] == '\\'));

        const DWORD loadLibraryFlags = LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;
        return reinterpret_cast<LibraryHandle>(LoadLibraryExA(path, nullptr, is_absolute ? LOAD_WITH_ALTERED_SEARCH_PATH : loadLibraryFlags));
#elif defined(__EMSCRIPTEN__)
        ALIMER_LOG_ERROR("Dynamic libraries are not supported on Emscripten.");
        return nullptr;
#elif defined(__linux__) || defined(__APPLE__)
        return dlopen(path, RTLD_NOW | RTLD_LOCAL);
#else
        return nullptr;
#endif
    }

    void CloseLibrary(LibraryHandle handle) noexcept
    {
        if (handle == nullptr)
            return;

#if defined(_WIN32)
        FreeLibrary(reinterpret_cast<HMODULE>(handle));
#elif defined(__EMSCRIPTEN__)
        ALIMER_LOG_ERROR("Dynamic libraries are not supported on Emscripten.");
#elif defined(__linux__) || defined(__APPLE__)
        dlclose(handle);
#endif
    }

    void* GetLibrarySymbol(LibraryHandle handle, const char* name) noexcept
    {
#if defined(_WIN32)
        return reinterpret_cast<void*>(GetProcAddress(reinterpret_cast<HMODULE>(handle), name));
#elif defined(__EMSCRIPTEN__)
        ALIMER_LOG_ERROR("Dynamic libraries are not supported on Emscripten.");
        return nullptr;
#elif defined(__linux__) || defined(__APPLE__)
        return dlsym(handle, name);
#else
        return nullptr;
#endif
    }
}
