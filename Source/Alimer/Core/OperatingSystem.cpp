// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/OperatingSystem.h"
#include "Alimer/Core/Assert.h"
#include "Alimer/Math/Math.h"

#if defined(_WIN32) || defined(_WIN64)
#include "Alimer/Platform/Win32/WindowsPlatform.h"
typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
#elif defined(__ANDROID__)
#include  <sys/system_properties.h>
#elif defined(__linux__)
#include <sys/utsname.h>
#elif defined(__APPLE__)
#include <sys/sysctl.h>
#include <SystemConfiguration/SystemConfiguration.h> // For the detection functions inside GetLoginName().
#endif
#include <thread>

using namespace Alimer;

PlatformId OperatingSystem::GetPlatformId()
{
#if ALIMER_PLATFORM_WINDOWS
    return PlatformId::Windows;
#elif ALIMER_PLATFORM_XBOX_ONE
    return PlatformId::XboxOne;
#elif ALIMER_PLATFORM_XBOX_SCARLETT
    return PlatformId::XboxScarlett;
#elif ALIMER_PLATFORM_LINUX
    return PlatformId::Linux;
#elif defined(__ANDROID__)
    return PlatformId::Android;
#elif ALIMER_PLATFORM_MACOS
    return PlatformId::MacOS;
#elif ALIMER_PLATFORM_IOS
    return PlatformId::iOS;
#elif defined(__EMSCRIPTEN__)
    return PlatformId::Browser;
#else
#   error "Unknown platform"
#endif
}

PlatformFamily OperatingSystem::GetPlatformFamily()
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

Version OperatingSystem::GetOSVersion()
{
    static Version osVersion(0, 0);
    if (osVersion.major == 0)
    {
#if defined(__EMSCRIPTEN__)
        osVersion = Version(1, 0, 0, 0);
#elif defined(_WIN32) || defined(_WIN64)
        HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
        if (hNtdll) {
            RtlGetVersionPtr fnRtlGetVersion =
                (RtlGetVersionPtr)GetProcAddress(hNtdll, "RtlGetVersion");
            if (fnRtlGetVersion) {
                RTL_OSVERSIONINFOW rovi = { 0 };
                rovi.dwOSVersionInfoSize = sizeof(rovi);
                if (fnRtlGetVersion(&rovi) == 0) { // STATUS_SUCCESS
                    osVersion.major = (int)rovi.dwMajorVersion;
                    osVersion.minor = (int)rovi.dwMinorVersion;
                    osVersion.build = (int)rovi.dwBuildNumber;
                }
            }
        }

        // 2. Get Revision (UBR - Update Build Revision) from Registry
        DWORD ubr = 0;
        DWORD dataSize = sizeof(ubr);
        if (RegGetValueW(
            HKEY_LOCAL_MACHINE,
            L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
            L"UBR",
            RRF_RT_REG_DWORD,
            NULL,
            &ubr,
            &dataSize) == ERROR_SUCCESS)
        {
            osVersion.revision = (int)ubr;
        }
#elif defined(__ANDROID__)
        char sdk_ver_str[PROP_VALUE_MAX] = { 0 };

        if (__system_property_get("ro.build.version.sdk_full", sdk_ver_str) > 0 ||
            __system_property_get("ro.build.version.sdk", sdk_ver_str) > 0)
        {
            // Parses up to 4 integers separated by dots.
            // If sdk_ver_str is "36.1", major=36, minor=1, build=0, revision=0.
            // If sdk_ver_str is "34", major=34, minor=0, build=0, revision=0.
            sscanf(sdk_ver_str, "%d.%d.%d.%d", &osVersion.major, &osVersion.minor, &osVersion.build, &osVersion.revision);
        }
#elif defined(__linux__)
        struct utsname uts;

        if (uname(&uts) == 0)
        {
            // Parses strings like "6.8.0-45-generic" -> major=6, minor=8, build=0, revision=45
            // Or "6.11.5" -> major=6, minor=11, build=5, revision=0
            sscanf(uts.release, "%d.%d.%d-%d", &osVersion.major, &osVersion.minor, &osVersion.build, &osVersion.revision);
        }
#elif defined(__APPLE__)
        char prod_ver[64] = { 0 };
        size_t size = sizeof(prod_ver);

        // 1. OS marketing/product version (e.g., "15.3.1" on macOS, "18.2" on iOS)
        if (sysctlbyname("kern.osproductversion", prod_ver, &size, NULL, 0) == 0) {
            sscanf(prod_ver, "%d.%d.%d", &osVersion.major, &osVersion.minor, &osVersion.build);
        }

        // 2. Optional: Kernel revision integer for the 4th field
        int os_rev = 0;
        size = sizeof(os_rev);
        if (sysctlbyname("kern.osrevision", &os_rev, &size, NULL, 0) == 0)
        {
            osVersion.revision = os_rev;
        }
#endif
    }

    return osVersion;
}

bool OperatingSystem::IsWindows()
{
#if defined(_WIN32) || defined(_WIN64)
    return true;
#else
    return false;
#endif
}

bool OperatingSystem::IsWindowsVersionAtLeast(int32_t major, int32_t minor, int32_t build, int32_t revision)
{
    return IsWindows() && IsOSVersionAtLeast(major, minor, build, revision);
}

bool OperatingSystem::IsBrowser()
{
#if defined(__EMSCRIPTEN__)
    return true;
#else
    return false;
#endif
}

bool OperatingSystem::IsLinux()
{
#if defined(__linux__) && !defined(__ANDROID__) && !defined(__EMSCRIPTEN__)
    return true;
#else
    return false;
#endif
}

bool OperatingSystem::IsAndroid()
{
#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

bool OperatingSystem::IsAndroidVersionAtLeast(int32_t major, int32_t minor, int32_t build, int32_t revision)
{
    return IsAndroid() && IsOSVersionAtLeast(major, minor, build, revision);
}

bool OperatingSystem::IsIOS()
{
    // TargetConditionals.h
#if defined(__APPLE__)
#if TARGET_OS_IOS || TARGET_OS_MACCATALYST
    return true;
#endif
#endif

    return false;
}

bool OperatingSystem::IsIOSVersionAtLeast(int32_t major, int32_t minor, int32_t build, int32_t revision)
{
    return IsIOS() && IsOSVersionAtLeast(major, minor, build, revision);
}

bool OperatingSystem::IsMacOS()
{
    // TargetConditionals.h
#if defined(__APPLE__)
#if TARGET_OS_MAC && !TARGET_OS_IOS && !TARGET_OS_MACCATALYST
    return true;
#endif
#endif

    return false;
}

bool OperatingSystem::IsMacOSVersionAtLeast(int32_t major, int32_t minor, int32_t build)
{
    return IsMacOS() && IsOSVersionAtLeast(major, minor, build, 0);
}

bool OperatingSystem::IsMacCatalyst()
{
    // TargetConditionals.h
#if defined(__APPLE__)
#if TARGET_OS_MACCATALYST
    return true;
#endif
#endif

    return false;
}

bool OperatingSystem::IsMacCatalystVersionAtLeast(int32_t major, int32_t minor , int32_t build)
{
    return IsMacCatalyst() && IsOSVersionAtLeast(major, minor, build, 0);
}

bool OperatingSystem::IsOSVersionAtLeast(int32_t major, int32_t minor, int32_t build, int32_t revision)
{
    Version current = GetOSVersion();

    if (current.major != major)
    {
        return current.major > major;
    }
    if (current.minor != minor)
    {
        return current.minor > minor;
    }

    // Unspecified build component is to be treated as zero
    int32_t currentBuild = current.build < 0 ? 0 : current.build;
    build = build < 0 ? 0 : build;
    if (currentBuild != build)
    {
        return currentBuild > build;
    }

    // Unspecified revision component is to be treated as zero
    int32_t currentRevision = current.revision < 0 ? 0 : current.revision;
    revision = revision < 0 ? 0 : revision;

    return currentRevision >= revision;
}


uint32_t OperatingSystem::GetNumPhysicalCPUs()
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

    return Math::Max(1u, cpuCount);
}
