// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/Base.h"

namespace Alimer
{
    class ALIMER_API Platform
    {
    public:
        /// Destructor.
        virtual ~Platform() = default;

        Platform(const Platform&) = delete;
        Platform& operator=(const Platform&) = delete;
        Platform(Platform&&) = delete;
        Platform& operator=(Platform&&) = delete;

        /// Create default platform for the current OS.
        [[nodiscard]] static Platform* CreateDefault();

    protected:
        Platform() = default;
    };
}
