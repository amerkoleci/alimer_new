// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Math/Math.h"

namespace Alimer
{
    struct ALIMER_API Version final
    {
    public:
        int32 major = 0;
        int32 minor = 0;
        int32 build = -1;
        int32 revision = -1;

        Version() noexcept = default;

        Version(int32 major_, int32 minor_) noexcept
            : major(Math::Max(major_, 0))
            , minor(Math::Max(minor_, 0))
            , build(-1)
            , revision(-1)
        {

        }

        Version(int32 major_, int32 minor_, int32 build_) noexcept
            : major(Math::Max(major_, 0))
            , minor(Math::Max(minor_, 0))
            , build(Math::Max(build_, 0))
            , revision(-1)
        {

        }

        Version(int32 major_, int32 minor_, int32 build_, int32 revision_) noexcept
            : major(Math::Max(major_, 0))
            , minor(Math::Max(minor_, 0))
            , build(Math::Max(build_, 0))
            , revision(Math::Max(revision_, 0))
        {

        }

        bool operator == (const Version& other) const noexcept { return major == other.major && minor == other.minor && build == other.build && revision == other.revision; }
        bool operator != (const Version& other) const noexcept { return !(*this == other); }
        bool operator>(const Version& other) const { return other < *this; }
        bool operator>=(const Version& other) const noexcept { return other <= *this; }
        bool operator<(const Version& other) const { return CompareTo(other) < 0; }
        bool operator<=(const Version& other) const { return CompareTo(other) <= 0; }

        /// Compare to another version.
        [[nodiscard]] int32 CompareTo(const Version& value) const;

        /// Return hash value of the vector.
        [[nodiscard]] size_t GetHashCode() const;

        /// Return as string.
        [[nodiscard]] String ToString() const;
    };
}

template<> struct std::hash<Alimer::Version>
{
    std::size_t operator()(const Alimer::Version& value) const noexcept
    {
        return value.GetHashCode();
    }
};
