// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/DynamicLibrary.h"

#if defined(_WIN32)
#include "Alimer/Platform/Win32/WindowsPlatform.h"
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif

using namespace Alimer;

DynamicLibrary::~DynamicLibrary()
{
    Unload();
}

bool DynamicLibrary::Load(StringView path) noexcept
{
    Unload();
    _handle = OpenLibrary(path.data());
    return IsLoaded();
}

void DynamicLibrary::Unload() noexcept
{
    if (_handle == nullptr)
        return;

    CloseLibrary(_handle);
    _handle = nullptr;
}
