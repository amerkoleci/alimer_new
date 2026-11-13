// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/String.h"

namespace Alimer::Assert
{
    // Custom assertion handler. Return true to break/trap, false to continue.
    using AssertHandler = bool(*)(const char* condition, const char* message, const char* file, int line, const char* function) noexcept;

    ALIMER_API AssertHandler GetHandler() noexcept;
    ALIMER_API void SetHandler(AssertHandler newHandler) noexcept;

    ALIMER_API bool ReportFailure(const char* expression, const char* message, const char* file, int line, const char* function) noexcept;

}

#ifndef ALIMER_ENABLE_ASSERT
#	if (defined(_DEBUG) || defined(PROFILE))
#		define ALIMER_ENABLE_ASSERT 1
#	else
#		define ALIMER_ENABLE_ASSERT 0
#	endif
#endif

#if ALIMER_ENABLE_ASSERT
#define ALIMER_ASSERT(cond) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (Alimer::Assert::ReportFailure(#cond, nullptr, __FILE__, __LINE__, __func__)) \
					ALIMER_BREAKPOINT; \
			} \
		} while(0)

#define ALIMER_ASSERT_MSG(cond, msg) \
		do \
		{ \
			if (!(cond)) \
			{ \
				if (Alimer::Assert::ReportFailure(#cond, msg, __FILE__, __LINE__, __func__)) \
					ALIMER_BREAKPOINT; \
			} \
		} while(0)

#define ALIMER_ASSERT_FAIL(msg) \
		do \
		{ \
			if (Alimer::Assert::ReportFailure(0, msg, __FILE__, __LINE__, __func__)) \
			    ALIMER_BREAKPOINT; \
		} while(0)

#define ALIMER_VERIFY(cond) ALIMER_ASSERT(cond)
#define ALIMER_VERIFY_MSG(cond, msg) ALIMER_ASSERT_MSG(cond, msg)

// Assert that will be called very often, like inside data structures e.g. operator[].
// Making it non-empty can make program slow.
#else
#   define ALIMER_ASSERT(condition) do { ALIMER_UNUSED(condition); } while(0)
#   define ALIMER_ASSERT_MSG(condition, msg) do { ALIMER_UNUSED(condition); ALIMER_UNUSED(msg); } while(0)
#   define ALIMER_ASSERT_FAIL(msg) do { ALIMER_UNUSED(msg); } while(0)
#   define ALIMER_VERIFY(cond) (void)(cond)
#   define ALIMER_VERIFY_MSG(cond, msg) do { (void)(cond); ALIMER_UNUSED(msg); } while(0)
#endif
