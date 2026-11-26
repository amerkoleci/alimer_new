// Copyright (c) Amer Koleci and Contributors.
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include <Alimer/Alimer.h>
#include <Alimer/EntryPoint.h>
using namespace Alimer;

class RHISamplesApp final : public Application
{
public:
    ~RHISamplesApp() override;

    void Setup() override;
    //void Initialize() override;
    //void Draw([[maybe_unused]] RHICommandBuffer* commandBuffer, [[maybe_unused]] RHITexture* outputTexture) override;
    //void OnWindowResized(Window* window) override;

private:
};

RHISamplesApp::~RHISamplesApp()
{
}

void RHISamplesApp::Setup()
{
    _options.name = "RHI Samples";
    //_options.graphics.preferredBackend = RHIBackend::Vulkan;
#if defined(_DEBUG)
    //_options.graphics.validationMode = RHIValidationMode::Enabled;
#endif
}

ALIMER_DEFINE_APPLICATION(RHISamplesApp);
