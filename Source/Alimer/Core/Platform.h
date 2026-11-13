// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Base.h"

namespace Alimer
{
    /// Identifiers the running platform type.
    enum class PlatformID
    {
        /// Windows platform.
        Windows,
        /// Linux platform.
        Linux,
        /// macOS platform.
        MacOS,
        /// Android platform.
        Android,
        /// iOS platform.
        iOS,
        /// Browser (WASM) platform.
        Browser,
        /// Xbox One platform.
        XboxOne,
        /// Xbox Series X|S platform.
        XboxScarlett,
    };

    /// Identifiers the running platform family.
    enum class PlatformFamily
    {
        /// Unknown family.
        Unknown,
        /// Mobile family.
        Mobile,
        /// Desktop family.
        Desktop,
        /// Console family.
        Console
    };


    ALIMER_API PlatformID GetPlatformID();
    ALIMER_API PlatformFamily GetPlatformFamily();
    ALIMER_API uint32_t GetNumPhysicalCPUs();

    /* Dynamic Library */
    using LibraryHandle = void*; // HMODULE on Win32
    ALIMER_API LibraryHandle OpenLibrary(const char* path) noexcept;
    ALIMER_API void CloseLibrary(LibraryHandle handle) noexcept;
    ALIMER_API void* GetLibrarySymbol(LibraryHandle handle, const char* name) noexcept;
}
