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
    ALIMER_API uint32_t RoundUpToPowerOf2(uint32_t value) noexcept;

    /// Rounds the specified value up to a power of two.
    ALIMER_API uint64_t RoundUpToPowerOf2(uint64_t value) noexcept;

    ALIMER_API uint32_t BitScanForward(uint32_t value);
    ALIMER_API uint32_t BitScanForward(uint64_t value);
    ALIMER_API uint32_t BitScanReverse(uint32_t value);
    ALIMER_API uint32_t BitScanReverse(uint64_t value);

    ALIMER_API uint32_t LeadingZeroCount(uint32_t value);
    ALIMER_API uint32_t LeadingZeroCount(uint64_t  value);

    ALIMER_API uint32_t Log2(uint32_t value);
    ALIMER_API uint32_t Log2(uint64_t value);

    ALIMER_API uint32_t PopCount(uint32_t value);
    ALIMER_API uint32_t PopCount(uint64_t value);

    /// Reverses the bits in an integer value
    ALIMER_API uint32_t ReverseBits(uint32_t value);

    /// Reverses the bits in an integer value
    ALIMER_API uint64_t ReverseBits(uint64_t value);

    ALIMER_API uint32_t RotateLeft(uint32_t value, uint32_t offset);
    ALIMER_API uint64_t RotateLeft(uint64_t value, uint32_t offset);
    ALIMER_API uint32_t RotateRight(uint32_t value, uint32_t offset);
    ALIMER_API uint64_t RotateRight(uint64_t value, uint32_t offset);

    ALIMER_API uint32_t TrailingZeroCount(uint32_t value);
    ALIMER_API uint32_t TrailingZeroCount(uint64_t value);
}
