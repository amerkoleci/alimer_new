// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/Assert.h"
#include "Alimer/Core/Log.h"

using namespace Alimer;

namespace
{
    bool DefaultHandler(const char* expression, const char* message, const char* file, int line, const char* function) noexcept
    {
        std::string printMessage = FMT::format("{}({}-{}): Assert Failure: ", file, line, function);

        if (expression != nullptr)
        {
            printMessage += FMT::format("'{}' ", expression);
        }

        if (message != nullptr && strlen(message) > 0)
        {
            printMessage += message;
        }

        Log::Critical(printMessage);
        return true; // break into the debugger / trap
    }

    Assert::AssertHandler& GetAssertHandlerInstance() noexcept
    {
        static Assert::AssertHandler s_handler = &DefaultHandler;
        return s_handler;
    }
}


Assert::AssertHandler Assert::GetHandler() noexcept
{
    return GetAssertHandlerInstance();
}

void Assert::SetHandler(AssertHandler newHandler) noexcept
{
    GetAssertHandlerInstance() = newHandler;
}

bool Assert::ReportFailure(const char* expression, const char* message, const char* file, int line, const char* function) noexcept
{
    return GetAssertHandlerInstance()(expression, message, file, line, function);
}
