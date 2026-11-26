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

        ALIMER_DISABLE_COPY_MOVE(Platform);

        /// Create default platform for the current OS.
        [[nodiscard]] static Platform* CreateDefault();

        virtual bool Initialize(const String& name) = 0;

    protected:
        Platform() = default;
    };
}
