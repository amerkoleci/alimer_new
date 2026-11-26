// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/Assert.h"
#include "Alimer/Core/BitOperations.h"
#include <type_traits>
#include <bit>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

using namespace Alimer;

uint32 BitOperations::RoundUpToPowerOf2(uint32 value) noexcept
{
    return std::bit_ceil(value);
}

uint64 BitOperations::RoundUpToPowerOf2(uint64 value) noexcept
{
    return std::bit_ceil(value);
}

uint32 BitOperations::BitScanForward(uint32 value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32>(std::countr_zero(value));
}

uint32 BitOperations::BitScanForward(uint64 value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32>(std::countr_zero(value));
}

uint32 BitOperations::BitScanReverse(uint32 value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32>(std::bit_width(value) - 1);
}

uint32 BitOperations::BitScanReverse(uint64 value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32>(std::bit_width(value) - 1);
}

uint32 BitOperations::LeadingZeroCount(uint32 value)
{
    return static_cast<uint32>(std::countl_zero(value));
}

uint32 BitOperations::LeadingZeroCount(uint64 value)
{
    return static_cast<uint32>(std::countl_zero(value));
}

uint32 BitOperations::Log2(uint32 value)
{
    return static_cast<uint32>(31 ^ std::countl_zero(value | 1u));
}

uint32 BitOperations::Log2(uint64 value)
{
    return static_cast<uint32>(31 ^ std::countl_zero(value | 1u));
}

uint32 BitOperations::PopCount(uint32 value)
{
    return static_cast<uint32>(std::popcount(value));
}

uint32 BitOperations::PopCount(uint64 value)
{
    return static_cast<uint32>(std::popcount(value));
}

uint32 BitOperations::ReverseBits(uint32 value)
{
    // std::byteswap is C++23, so we implement a manual bit reversal for compatibility with earlier standards.
#if defined(__clang__)
    return __builtin_bitreverse32(value);
#else
    value = ((value >> 1) & 0x55555555u) | ((value & 0x55555555u) << 1);
    value = ((value >> 2) & 0x33333333u) | ((value & 0x33333333u) << 2);
    value = ((value >> 4) & 0x0F0F0F0Fu) | ((value & 0x0F0F0F0Fu) << 4);

    if (!std::is_constant_evaluated())
    {
#if defined(_MSC_VER)
        return _byteswap_ulong(value);
#elif defined(__GNUC__)
        return __builtin_bswap32(value);
#endif
    }

    value = ((value >> 8) & 0x00FF00FFu) | ((value & 0x00FF00FFu) << 8);
    return (value >> 16) | (value << 16);
#endif
}

uint64 BitOperations::ReverseBits(uint64 value)
{
    // std::byteswap is C++23, so we implement a manual bit reversal for compatibility with earlier standards.
#if defined(__clang__)
    return __builtin_bitreverse64(value);
#else
    value = ((value >> 1) & 0x5555555555555555ull) | ((value & 0x5555555555555555ull) << 1);
    value = ((value >> 2) & 0x3333333333333333ull) | ((value & 0x3333333333333333ull) << 2);
    value = ((value >> 4) & 0x0F0F0F0F0F0F0F0Full) | ((value & 0x0F0F0F0F0F0F0F0Full) << 4);

    if (!std::is_constant_evaluated())
    {
#if defined(_MSC_VER)
        return _byteswap_uint64(value);
#elif defined(__GNUC__)
        return __builtin_bswap64(value);
#endif
    }

    value = ((value >> 8) & 0x00FF00FF00FF00FFull) | ((value & 0x00FF00FF00FF00FFull) << 8);
    value = ((value >> 16) & 0x0000FFFF0000FFFFull) | ((value & 0x0000FFFF0000FFFFull) << 16);
    return (value >> 32) | (value << 32);
#endif
}

uint32 BitOperations::RotateLeft(uint32 value, uint32 offset)
{
    return std::rotl(value, static_cast<int>(offset));
}

uint64 BitOperations::RotateLeft(uint64 value, uint32 offset)
{
    return std::rotl(value, static_cast<int>(offset));
}

uint32 BitOperations::RotateRight(uint32 value, uint32 offset)
{
    return std::rotr(value, static_cast<int>(offset));
}

uint64 BitOperations::RotateRight(uint64 value, uint32 offset)
{
    return std::rotr(value, static_cast<int>(offset));
}

uint32 BitOperations::TrailingZeroCount(uint32 value)
{
    return static_cast<uint32>(std::countr_zero(value));
}

uint32 BitOperations::TrailingZeroCount(uint64 value)
{
    return static_cast<uint32>(std::countr_zero(value));
}

uint32 BitOperations::SingleToUInt32Bits(float value)
{
    return std::bit_cast<uint32>(value);
}

float BitOperations::UInt32BitsToSingle(uint32 value)
{
    return std::bit_cast<float>(value);
}

double BitOperations::UInt64BitsToDouble(uint64 value)
{
    return std::bit_cast<double>(value);
}
