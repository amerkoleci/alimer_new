// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Math/Math.h"
#include <bit>

#if __has_include(<stdfloat>)
#include <stdfloat>
#endif

using namespace Alimer;

float Math::CopySign(float x, float y)
{
    //return std::copysign(x, y);
    // std::copysign
    const uint32_t SignMask = 0x80000000;

    // Use bit manipulation to copy the sign bit from y to x
    uint32_t xBits = std::bit_cast<uint32_t>(x);
    uint32_t yBits = std::bit_cast<uint32_t>(y);
    // Clear the sign bit of x and set it to the sign bit of y
    xBits = (xBits & 0x7FFFFFFF) | (yBits & SignMask);
    return  std::bit_cast<float>(xBits);
}

float Math::HalfToFloat(Half value) noexcept
{
#if defined(__STDCPP_FLOAT16_T__)
    // Native C++23 hardware-accelerated conversion when supported by the target
    return static_cast<float>(std::bit_cast<std::float16_t>(value));
#else
    const uint32_t sign = (static_cast<uint32_t>(value) & 0x8000u) << 16;
    uint32_t exponent = (value & 0x7C00u) >> 10;
    uint32_t mantissa = value & 0x03FFu;

    if (exponent == 0x1Fu) // Infinity or NaN
    {
        exponent = 0xFFu;
    }
    else if (exponent != 0u) // Normalized number
    {
        exponent += 112u; // Adjust bias from 15 (FP16) to 127 (FP32)
    }
    else if (mantissa != 0u) // Subnormal (denormalized) number
    {
        // Count leading zeros to normalize the mantissa without a loop.
        // A uint16_t has 16 bits, and the mantissa occupies the lower 10 bits.
        const int shift = std::countl_zero(static_cast<uint16_t>(mantissa)) - 5;
        mantissa = (mantissa << shift) & 0x03FFu;
        exponent = 113u - static_cast<uint32_t>(shift);
    }
    // If exponent == 0 && mantissa == 0, both remain 0 (preserves +0.0f and -0.0f)

    const uint32_t result = sign | (exponent << 23) | (mantissa << 13);
    return std::bit_cast<float>(result);
#endif
}

Half Math::FloatToHalf(float value) noexcept
{
#if defined(__STDCPP_FLOAT16_T__)
    // Native C++23 conversion when supported by the target platform
    return std::bit_cast<Half>(static_cast<std::float16_t>(value));
#else
    uint32_t result = 0;

    // Safely reinterpret float bits without violating strict aliasing
    uint32_t intValue = std::bit_cast<uint32_t>(value);
    const uint32_t sign = (intValue & 0x80000000u) >> 16u;
    intValue &= 0x7FFFFFFFu; // Clear the sign bit

    if (intValue >= 0x47800000u) // >= 2^16 (65536.0f)
    {
        // Value is too large to be represented as a half: return Infinity or Quiet NaN
        result = 0x7C00u | ((intValue > 0x7F800000u) ? (0x0200u | ((intValue >> 13u) & 0x03FFu)) : 0u);
    }
    else if (intValue <= 0x33000000u) // <= 2^-25
    {
        // Value underflows to zero
        result = 0u;
    }
    else if (intValue < 0x38800000u) // < 2^-14
    {
        // Value is too small for a normalized half: convert to subnormal (denormalized) with round-to-nearest-even
        const uint32_t shift = 125u - (intValue >> 23u);
        intValue = 0x00800000u | (intValue & 0x007FFFFFu); // Add implicit leading 1 bit
        result = intValue >> (shift + 1u);

        const uint32_t sticky = (intValue & ((1u << shift) - 1u)) != 0u ? 1u : 0u;
        result += (result | sticky) & ((intValue >> shift) & 1u);
    }
    else
    {
        // Rebias exponent from 127 to 15 (add -112 in two's complement: 0xC8000000) and round-to-nearest-even
        intValue += 0xC8000000u;
        result = ((intValue + 0x0FFFu + ((intValue >> 13u) & 1u)) >> 13u) & 0x7FFFu;
    }

    return static_cast<Half>(result | sign);
#endif
}

float Math::LinearToSRgb(float linearValue) noexcept
{
    if (linearValue <= 0.0f)
        return 0.0f;
    else if (linearValue <= 0.0031308f)
        return 12.92f * linearValue;
    else if (linearValue < 1.0f)
        return 1.055f * std::pow(linearValue, 0.4166667f) - 0.055f;
    else
        return std::pow(linearValue, 0.45454545f);
}


float Math::SRgbToLinear(float sRgbValue) noexcept
{
    if (sRgbValue <= 0.04045f)
        return sRgbValue / 12.92f;
    else if (sRgbValue < 1.0f)
        return std::pow((sRgbValue + 0.055f) / 1.055f, 2.4f);
    else
        return std::pow(sRgbValue, 2.2f);
}
