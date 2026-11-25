// Copyright (c) Amer Koleci and Contributors.
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include <Alimer/Alimer.h>
#include <Alimer/EntryPoint.h>
using namespace Alimer;

class RHISamplesApp final : public Application
{
public:
    ~RHISamplesApp() override;

    //void Setup() override;
    //void Initialize() override;
    //void Draw([[maybe_unused]] RHICommandBuffer* commandBuffer, [[maybe_unused]] RHITexture* outputTexture) override;
    //void OnWindowResized(Window* window) override;

private:
};

RHISamplesApp::~RHISamplesApp()
{
}

ALIMER_DEFINE_APPLICATION(RHISamplesApp);
