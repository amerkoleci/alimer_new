// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/Version.h"
#include "Alimer/Core/Hash.h"

using namespace Alimer;

int32_t Version::CompareTo(const Version& value) const
{
    return
        major != value.major ? (major > value.major ? 1 : -1) :
        minor != value.minor ? (minor > value.minor ? 1 : -1) :
        build != value.build ? (build > value.build ? 1 : -1) :
        revision != value.revision ? (revision > value.revision ? 1 : -1) :
        0;
}

size_t Version::GetHashCode() const
{
    size_t hash = 0;
    HashCombine(hash, major, minor, build, revision);
    return hash;
}


String Version::ToString() const
{
    const uint32_t fieldCount = build == -1 ? 2 : revision == -1 ? 3 : 4;
    switch (fieldCount)
    {
        case 2:
            return FMT::format("{}.{}", major, minor);
        case 3:
            return FMT::format("{}.{}.{}", major, minor, build);
        case 4:
            return FMT::format("{}.{}.{}.{}", major, minor, build, revision);
        default:
            return kEmptyString;
    }
}
