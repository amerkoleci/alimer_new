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

    class ALIMER_API OperatingSystem
    {
    public:
        [[nodiscard]] static PlatformID GetPlatformID();
        [[nodiscard]] static PlatformFamily GetPlatformFamily();
        [[nodiscard]] static uint32_t GetNumPhysicalCPUs();
    };
}
