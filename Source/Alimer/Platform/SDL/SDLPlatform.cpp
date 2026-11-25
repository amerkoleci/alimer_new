// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "SDLPlatform.h"
#include "Alimer/Core/Log.h"
#include <SDL3/SDL.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL_main.h>

using namespace Alimer;

namespace
{
    static void AlimerLog_SDL(void* /*userdata*/, int /*category*/, SDL_LogPriority priority, const char* message)
    {
        switch (priority)
        {
            case SDL_LOG_PRIORITY_VERBOSE:
                Log::Trace(message);
                break;
            case SDL_LOG_PRIORITY_DEBUG:
                Log::Debug(message);
                break;
            case SDL_LOG_PRIORITY_INFO:
                Log::Info(message);
                break;
            case SDL_LOG_PRIORITY_WARN:
                Log::Warn(message);
                break;
            case SDL_LOG_PRIORITY_ERROR:
                Log::Error(message);
                break;
            case SDL_LOG_PRIORITY_CRITICAL:
                Log::Critical(message);
                break;
            default:
                break;
        }
    }
}

SDLPlatform::SDLPlatform()
{
    SDL_SetMainReady();
#if defined(_DEBUG)
    SDL_SetLogPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_DEBUG);
#endif
    SDL_SetLogOutputFunction(AlimerLog_SDL, nullptr);

    // Init SDL
    const Uint32 initFlags = SDL_INIT_VIDEO | SDL_INIT_GAMEPAD;
    if (!SDL_Init(initFlags))
    {
        LOGE("Alimer: SDL_Init Failed: {}", SDL_GetError());
        return;
    }
    SDL_AddEventWatch(&SDLPlatform::OnWatchedEvent, this);

    //if (!_options.name.empty())
    //{
    //    SDL_SetHint(SDL_HINT_APP_NAME, _options.name.c_str());
    //}

    const int version = SDL_GetVersion();
    [[ maybe_unused ]] const char* revision = SDL_GetRevision();
    LOGI("SDL Initialized: v{}.{}.{}",
        SDL_VERSIONNUM_MAJOR(version),
        SDL_VERSIONNUM_MINOR(version),
        SDL_VERSIONNUM_MICRO(version)
    );
}

SDLPlatform::~SDLPlatform()
{
    SDL_RemoveEventWatch(&SDLPlatform::OnWatchedEvent, this);
    SDL_Quit();
}

bool SDLPlatform::OnWatchedEvent(void* userdata, SDL_Event* event)
{
    SDLPlatform& platform = *static_cast<SDLPlatform*>(userdata);
    return true;
}

Platform* Platform::CreateDefault()
{
    return new SDLPlatform();
}
