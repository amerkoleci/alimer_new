// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Version.h"

namespace Alimer
{
    /// Identifiers the running platform type.
    enum class PlatformId
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
        [[nodiscard]] static PlatformId GetPlatformId();
        [[nodiscard]] static PlatformFamily GetPlatformFamily();
        [[nodiscard]] static Version GetOSVersion();

        /// Indicates whether the current application is running on Windows.
        [[nodiscard]] static bool IsWindows();
        [[nodiscard]] static bool IsWindowsVersionAtLeast(int32 major, int32 minor = 0, int32 build = 0, int32 revision = 0);

        /// Indicates whether the current application is running as WASM in a Browser.
        [[nodiscard]] static bool IsBrowser();

        /// Indicates whether the current application is running on Linux.
        [[nodiscard]] static bool IsLinux();

        /// Indicates whether the current application is running on Android.
        [[nodiscard]] static bool IsAndroid();

        /// Check for the Android API level (returned by 'ro.build.version.sdk') with a >= version comparison. Used to guard APIs that were added in the given Android release.
        [[nodiscard]] static bool IsAndroidVersionAtLeast(int32 major, int32 minor = 0, int32 build = 0, int32 revision = 0);

        /// Indicates whether the current application is running on iOS or MacCatalyst.
        [[nodiscard]] static bool IsIOS();

        /// Check for the iOS version with a >= version comparison. Used to guard APIs that were added in the given iOS release.
        [[nodiscard]] static bool IsIOSVersionAtLeast(int32 major, int32 minor = 0, int32 build = 0, int32 revision = 0);

        /// Indicates whether the current application is running on macOS.
        [[nodiscard]] static bool IsMacOS();

        [[nodiscard]] static bool IsMacOSVersionAtLeast(int32 major, int32 minor = 0, int32 build = 0);

        /// Indicates whether the current application is running on Mac Catalyst.
        [[nodiscard]] static bool IsMacCatalyst();

        /// Check for the Mac Catalyst version (iOS version as presented in Apple documentation) with a >= version comparison. Used to guard APIs that were added in the given Mac Catalyst release.
        [[nodiscard]] static bool IsMacCatalystVersionAtLeast(int major, int minor = 0, int build = 0);

        [[nodiscard]] static uint32_t GetNumPhysicalCPUs();

    private:
        [[nodiscard]] static bool IsOSVersionAtLeast(int32 major, int32 minor, int32 build, int32 revision);
    };
}
