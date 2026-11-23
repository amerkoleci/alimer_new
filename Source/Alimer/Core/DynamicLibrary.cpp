// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/DynamicLibrary.h"
#include "Alimer/Core/Log.h"
#include "Alimer/Core/Assert.h"

#if defined(_WIN32)
#include "Alimer/Platform/Win32/WindowsPlatform.h"
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

using namespace Alimer;

DynamicLibrary::~DynamicLibrary()
{
    Close();
}

DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept
{
    std::swap(_handle, other._handle);
    _needsClose = other._needsClose;
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept
{
    std::swap(_handle, other._handle);
    _needsClose = other._needsClose;
    return *this;
}

bool DynamicLibrary::Open(StringView path, String* error) noexcept
{
    Close();

#if defined(_WIN32)
    // Use SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS to avoid DLL search path attacks.
    const bool is_absolute = path != nullptr && ((path[0] != '\0' && path[1] == ':') || (path[0] == '\\' && path[1] == '\\'));

    const DWORD loadLibraryFlags = LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;
    HMODULE handle = LoadLibraryExA(path.data(), nullptr, is_absolute ? LOAD_WITH_ALTERED_SEARCH_PATH : loadLibraryFlags);
    if (handle == nullptr)
    {
        if (error != nullptr)
        {
            DWORD errorCode = GetLastError();
            *error = "Failed to load library '{}'. Error code: {}", path, errorCode;
        }

        _handle = nullptr;
        return false;
    }
    _handle = reinterpret_cast<LibraryHandle>(handle);
#elif defined(__linux__) || defined(__APPLE__)
    _handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (_handle == nullptr && error != nullptr)
    {
        *error = dlerror();
    }
#else
    if (error != nullptr)
    {
        *error = "Unsupported platform for DynamicLibrary";
    }

    return false;
#endif

    _needsClose = _handle != nullptr;
    return _needsClose;
}

void DynamicLibrary::Close() noexcept
{
    if (_handle == nullptr)
    {
        return;
    }

    // If the dynamic library was opened with OpenLoaded don't attempt to close it.
    if (_needsClose)
    {
#if defined(_WIN32)
        FreeLibrary(static_cast<HMODULE>(_handle));
#elif defined(__linux__) || defined(__APPLE__)
        dlclose(_handle);
#else
        LOGE("Unsupported platform for DynamicLibrary");
#endif
    }

    _needsClose = false;
    _handle = nullptr;
}

void* DynamicLibrary::GetExport(StringView name, String* error) const noexcept
{
    void* proc = nullptr;

#if defined(_WIN32)
    proc = reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(_handle), name.data()));

    if (proc == nullptr && error != nullptr) {
        *error = "Windows Error: " + std::to_string(GetLastError());
    }
#elif defined(__linux__) || defined(__APPLE__)
    proc = reinterpret_cast<void*>(dlsym(_handle, name.data()));

    if (proc == nullptr && error != nullptr) {
        *error = dlerror();
    }
#else
    if (error != nullptr)
    {
        *error = "Unsupported platform for DynamicLibrary";
    }

    return nullptr;
#endif

    return proc;
}
