// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#include "Alimer/Core/Assert.h"
#include "Alimer/Core/Log.h"
#include "Alimer/Application.h"
#include "Alimer/Platform/Platform.h"
#include <thread>

using namespace Alimer;

std::thread::id mainThreadId = std::this_thread::get_id();
Application* Application::s_Instance = nullptr;

Application& Application::Get()
{
    return *s_Instance;
}

bool Application::IsMainThread()
{
    return std::this_thread::get_id() == mainThreadId;
}

Application::Application(Platform* platform)
    : _platform(platform ? platform : Platform::CreateDefault())
{
    ALIMER_ASSERT_MSG(s_Instance == nullptr, "Cannot create more than one Application");
    Log::Init();
    //JobSystem::Initialize();
    mainThreadId = std::this_thread::get_id();

    s_Instance = this;
}

Application::~Application()
{
    Log::Shutdown();
    //JobSystem::Shutdown();
    SafeDelete(_platform);
    s_Instance = nullptr;
}

void Application::Run()
{
    // Allow config override.
    Setup();

    // Init platform first
    if (!_platform->Initialize(_options.name))
    {
        LOGF("Failed to initialize platform");
        return;
    }
}

Application& Alimer::GetApplication()
{
    return Application::Get();
}
