// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Math/Math.h"
#include <bit>

using namespace Alimer;

float Math::CopySign(float x, float y)
{
    //return std::copysign(x, y);
    // std::copysign
    const uint32_t SignMask = 0x80000000;

    // Use bit manipulation to copy the sign bit from y to x
    uint32 xBits = std::bit_cast<uint32>(x);
    uint32 yBits = std::bit_cast<uint32>(y);
    // Clear the sign bit of x and set it to the sign bit of y
    xBits = (xBits & 0x7FFFFFFF) | (yBits & SignMask);
    return  std::bit_cast<float>(xBits);
}
