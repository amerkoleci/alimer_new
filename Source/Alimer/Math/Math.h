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

    [[nodiscard]] inline bool IsFinite(float value) { return std::isfinite(value); }

    /// Check whether a floating point value is positive or negative infinity.
    [[nodiscard]] inline bool IsInf(float value) { return std::isinf(value); }

    /// Check whether a floating point value is NaN.
    [[nodiscard]] inline bool IsNaN(float value) { return std::isnan(value); }

    /// Determines whether the specified value is negative.
    [[nodiscard]] inline bool IsNegative(float value) { return std::signbit(value); }

    /// Determines whether the specified value is negative infinity.
    [[nodiscard]] inline bool IsNegativeInfinity(float f) { return std::isinf(f) && std::signbit(f); }

    /// Determines whether the specified value is normal (finite, but not zero or subnormal).
    [[nodiscard]] inline bool IsNormal(float value) { return std::fpclassify(value) == FP_NORMAL; }

    /// Determines whether the specified value is positive infinity.
    [[nodiscard]] inline bool IsPositiveInfinity(float f) { return std::isinf(f) && !std::signbit(f); }

    /// Determines whether the specified value is subnormal (finite, but not zero or normal).
    [[nodiscard]] inline bool IsSubnormal(float value) { return std::fpclassify(value) == FP_SUBNORMAL; }

    [[nodiscard]] constexpr bool IsZero(float value) noexcept { return Abs(value) < kEpsilon; }
    [[nodiscard]] constexpr bool IsOne(float value) noexcept { return Abs(value - 1.0f) < kEpsilon; }

    /// Check whether one float is equal within accuracy.
    [[nodiscard]] constexpr bool NearEqual(float value, float epsilon = kEpsilon) noexcept { return Abs(value) <= epsilon; }

    /// Check whether two float values are equal within accuracy.
    [[nodiscard]] constexpr bool NearEqual(float lhs, float rhs, float epsilon = kEpsilon) noexcept
    {
        return Abs(lhs - rhs) <= epsilon;
    }

    /// Return the sign of a float (-1, 0 or 1).
    [[nodiscard]] constexpr float Sign(float value) { return value > 0 ? 1.0f : (value < 0 ? -1.0f : 0.0f); }

    [[nodiscard]] inline float Sqrt(float value) noexcept { return std::sqrt(value); }
    [[nodiscard]] inline float InvSqrt(float value) noexcept { return 1.0f / std::sqrt(value); }
    [[nodiscard]] inline float Sin(float value) noexcept { return std::sin(value); }
    [[nodiscard]] inline float Cos(float value) noexcept { return std::cos(value); }
    [[nodiscard]] inline float Tan(float value) noexcept { return std::tan(value); }
    [[nodiscard]] inline float Asin(float value) noexcept { return std::asin(value); }
    [[nodiscard]] inline float Acos(float value) noexcept { return std::acos(value); }
    [[nodiscard]] inline float Atan(float value) noexcept { return std::atan(value); }
    [[nodiscard]] inline float Atan2(float y, float x) noexcept { return std::atan2(y, x); }
    [[nodiscard]] inline float Round(float value) noexcept { return std::round(value); }
    [[nodiscard]] inline float Floor(float value) noexcept { return std::floor(value); }
    [[nodiscard]] inline float Ceil(float value) noexcept { return std::ceil(value); }
    [[nodiscard]] inline float Trunc(float value) noexcept { return std::trunc(value); }

    [[nodiscard]] inline float Pow(float base, float exp) noexcept { return std::pow(base, exp); }
    [[nodiscard]] inline float Exp(float x) noexcept { return std::exp(x); }

    [[nodiscard]] inline float Log(float x) noexcept { return std::log(x); } // natural log
    [[nodiscard]] inline float Log2(float x) noexcept { return std::log2(x); }
    [[nodiscard]] inline float Log10(float x) noexcept { return std::log10(x); }

    [[nodiscard]] constexpr float DegreesToRadians(float degrees) noexcept { return degrees * kDegToRad; }
    [[nodiscard]] constexpr float RadiansToDegrees(float radians) noexcept { return radians * kRadToDeg; }

    /// Linear interpolation between two values.
    [[nodiscard]] constexpr float Lerp(float lhs, float rhs, float t) noexcept
    {
        return lhs * (1.0f - t) + rhs * t;
    }

    /// Inverse linear interpolation between two values.
    [[nodiscard]] constexpr float InverseLerp(float lhs, float rhs, float x) noexcept
    {
        return (x - lhs) / (rhs - lhs);
    }

    /// Performs smooth (cubic Hermite) interpolation between 0 and 1.
    [[nodiscard]] constexpr float SmoothStep(float amount) noexcept
    {
        return (amount <= 0) ? 0 : (amount >= 1) ? 1 : amount * amount * (3 - (2 * amount));
    }

    ALIMER_API float CopySign(float x, float y);

    /// Converts a 16-bit floating point value to a 32-bit floating point value.
    ALIMER_API float HalfToFloat(Half value) noexcept;

    /// Converts a 32-bit floating point value to a 16-bit floating point value.
    ALIMER_API Half FloatToHalf(float value) noexcept;

    /// Converts a float value from linear to sRGB (gamma) space.
    ALIMER_API float LinearToSRgb(float linearValue) noexcept;

    /// Convert a float value from sRGB ( gamma) to linear.
    ALIMER_API float SRgbToLinear(float sRgbValue) noexcept;
}
