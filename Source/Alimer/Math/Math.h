// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Assert.h"
#include <stdlib.h>
#include <cmath>
#include <limits>

namespace Alimer::Math
{
    // =======================================================================
    // Constants & scalar functions
    // =======================================================================
    inline constexpr float kPi = 3.14159265358979323846f;
    inline constexpr float kTwoPi = 2.0f * kPi;
    inline constexpr float kHalfPi = 0.5f * kPi;
    inline constexpr float kInvPi = 1.0f / kPi;
    inline constexpr float kDegToRad = kPi / 180.0f;
    inline constexpr float kRadToDeg = 180.0f / kPi;
    inline constexpr float kEpsilon = 1.0e-6f;
    inline constexpr float kFloatMax = 3.402823466e38f;

    //---------------------------------------------
    // Basic comparisons
    //---------------------------------------------
    template <typename T> [[nodiscard]] constexpr T Abs(T v) { return (v >= 0) ? v : -v; }
    template <typename T> [[nodiscard]] constexpr T Min(T a, T b) { return (a < b) ? a : b; }
    template <typename T> [[nodiscard]] constexpr T Max(T a, T b) { return (a < b) ? b : a; }
    template <typename T> [[nodiscard]] constexpr T Clamp(T arg, T lo, T hi) { return (arg < lo) ? lo : (arg < hi) ? arg : hi; }

    [[nodiscard]] constexpr float DegreesToRadians(float degrees) noexcept { return degrees * kDegToRad; }
    [[nodiscard]] constexpr float RadiansToDegrees(float radians) noexcept { return radians * kRadToDeg; }

    ALIMER_API float CopySign(float x, float y);
}
