// Copyright (c) Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#pragma once

#include "Alimer/Core/RefCounted.h"

namespace Alimer
{
    class Platform;

    struct ApplicationOptions final
    {
        /// Application name.
        String name = "Alimer";
    };

    /// Class that provides graphics initialization, application logic, and rendering code.
    class ALIMER_API Application : public RefCounted
    {
    public:
        /// Destructor.
        virtual ~Application();

        ALIMER_DISABLE_COPY_MOVE(Application);

        static Application& Get();
        static bool IsMainThread();

        /// Setups all subsystem and run's platform main loop.
        void Run();

    protected:
        /// Constructor.
        Application(Platform* platform = nullptr);

        /// Setup is called before platform initialization. Override this function to setup your application.
        virtual void Setup() {}

    protected:
        Platform* _platform{ nullptr };
        ApplicationOptions _options{};

    private:
        static Application* s_Instance;
    };

    /** Provides easier access to Application. */
    ALIMER_API Application& GetApplication();
}
