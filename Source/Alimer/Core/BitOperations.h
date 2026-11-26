// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Assert.h"

namespace Alimer::BitOperations
{
    /// Returns true if value is a power of two.
    template <typename T>
    constexpr bool IsPow2(T value)
    {
        return (value & (value - 1)) == 0;
    }

    /// Aligns value up to the given alignment. Assumes alignment is power of two.
    template <typename T>
    constexpr T AlignUp(T value, T alignment)
    {
        ALIMER_ASSERT(IsPow2(alignment));

        return (value + alignment - 1) & ~(alignment - 1);
    }

    template <typename T>
    constexpr T AlignDown(T value, T alignment)
    {
        ALIMER_ASSERT(IsPow2(alignment));
        return value & ~(alignment - 1);
    }

    template <typename T>
    constexpr bool IsAligned(T value, size_t alignment)
    {
        return 0 == ((size_t)value & (alignment - 1));
    }

    template<typename T>
    constexpr T Align(T value, T alignment)
    {
        return ((value + alignment - T(1)) / alignment) * alignment;
    }

    template <typename T>
    constexpr T DivideByMultiple(T value, size_t alignment)
    {
        return (T)((value + alignment - 1) / alignment);
    }

    /// Rounds the specified value up to a power of two.
    ALIMER_API uint32 RoundUpToPowerOf2(uint32 value) noexcept;

    /// Rounds the specified value up to a power of two.
    ALIMER_API uint64 RoundUpToPowerOf2(uint64 value) noexcept;

    ALIMER_API uint32 BitScanForward(uint32 value);
    ALIMER_API uint32 BitScanForward(uint64 value);
    ALIMER_API uint32 BitScanReverse(uint32 value);
    ALIMER_API uint32 BitScanReverse(uint64 value);

    ALIMER_API uint32 LeadingZeroCount(uint32 value);
    ALIMER_API uint32 LeadingZeroCount(uint64  value);

    ALIMER_API uint32 Log2(uint32 value);
    ALIMER_API uint32 Log2(uint64 value);

    ALIMER_API uint32 PopCount(uint32 value);
    ALIMER_API uint32 PopCount(uint64 value);

    /// Reverses the bits in an integer value
    ALIMER_API uint32 ReverseBits(uint32 value);

    /// Reverses the bits in an integer value
    ALIMER_API uint64 ReverseBits(uint64 value);

    ALIMER_API uint32 RotateLeft(uint32 value, uint32 offset);
    ALIMER_API uint64 RotateLeft(uint64 value, uint32 offset);
    ALIMER_API uint32 RotateRight(uint32 value, uint32 offset);
    ALIMER_API uint64 RotateRight(uint64 value, uint32 offset);

    ALIMER_API uint32 TrailingZeroCount(uint32 value);
    ALIMER_API uint32 TrailingZeroCount(uint64 value);

    ALIMER_API uint32 SingleToUInt32Bits(float value);
    ALIMER_API float UInt32BitsToSingle(uint32 value);
    ALIMER_API double UInt64BitsToDouble(uint64 value);
}
