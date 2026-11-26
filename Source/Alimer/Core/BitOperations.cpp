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

uint32_t BitOperations::RoundUpToPowerOf2(uint32_t value) noexcept
{
    return std::bit_ceil(value);
}

uint64_t BitOperations::RoundUpToPowerOf2(uint64_t value) noexcept
{
    return std::bit_ceil(value);
}

uint32_t BitOperations::BitScanForward(uint32_t value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32_t>(std::countr_zero(value));
}

uint32_t BitOperations::BitScanForward(uint64_t value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32_t>(std::countr_zero(value));
}

uint32_t BitOperations::BitScanReverse(uint32_t value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32_t>(std::bit_width(value) - 1);
}

uint32_t BitOperations::BitScanReverse(uint64_t value)
{
    ALIMER_ASSERT(value != 0);

    return static_cast<uint32_t>(std::bit_width(value) - 1);
}

uint32_t BitOperations::LeadingZeroCount(uint32_t value)
{
    return static_cast<uint32_t>(std::countl_zero(value));
}

uint32_t BitOperations::LeadingZeroCount(uint64_t value)
{
    return static_cast<uint32_t>(std::countl_zero(value));
}

uint32_t BitOperations::Log2(uint32_t value)
{
    return static_cast<uint32_t>(31 ^ std::countl_zero(value | 1u));
}

uint32_t BitOperations::Log2(uint64_t value)
{
    return static_cast<uint32_t>(31 ^ std::countl_zero(value | 1u));
}

uint32_t BitOperations::PopCount(uint32_t value)
{
    return static_cast<uint32_t>(std::popcount(value));
}

uint32_t BitOperations::PopCount(uint64_t value)
{
    return static_cast<uint32_t>(std::popcount(value));
}

uint32_t BitOperations::ReverseBits(uint32_t value)
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

uint64_t BitOperations::ReverseBits(uint64_t value)
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

uint32_t BitOperations::RotateLeft(uint32_t value, uint32_t offset)
{
    return std::rotl(value, static_cast<int>(offset));
}

uint64_t BitOperations::RotateLeft(uint64_t value, uint32_t offset)
{
    return std::rotl(value, static_cast<int>(offset));
}

uint32_t BitOperations::RotateRight(uint32_t value, uint32_t offset)
{
    return std::rotr(value, static_cast<int>(offset));
}

uint64_t BitOperations::RotateRight(uint64_t value, uint32_t offset)
{
    return std::rotr(value, static_cast<int>(offset));
}

uint32_t BitOperations::TrailingZeroCount(uint32_t value)
{
    return static_cast<uint32_t>(std::countr_zero(value));
}

uint32_t BitOperations::TrailingZeroCount(uint64_t value)
{
    return static_cast<uint32_t>(std::countr_zero(value));
}

uint32_t BitOperations::SingleToUInt32Bits(float value)
{
    return std::bit_cast<uint32_t>(value);
}

float BitOperations::UInt32BitsToSingle(uint32_t value)
{
    return std::bit_cast<float>(value);
}

double BitOperations::UInt64BitsToDouble(uint64_t value)
{
    return std::bit_cast<double>(value);
}
