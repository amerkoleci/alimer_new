// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Platform/Platform.h"
union SDL_Event; // the event watch's argument

namespace Alimer
{
    class ALIMER_API SDLPlatform : public Platform
    {
    public:
        SDLPlatform();
        ~SDLPlatform() override;

    private:
        static bool OnWatchedEvent(void* userdata, SDL_Event* event);
    };
}
