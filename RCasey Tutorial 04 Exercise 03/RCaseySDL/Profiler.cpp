#include "Profiler.h"
#include <iostream>
#include <vector>





/// <summary>
/// Initializes a new instance of the <see cref="ProfilerSystem" /> class.
/// </summary>
ProfilerSystem::ProfilerSystem()
{
    currentFrameTime = 0;
}

/// <summary>
/// Finalizes an instance of the <see cref="ProfilerSystem" /> class.
/// </summary>
ProfilerSystem::~ProfilerSystem()
{
    
}

/// <summary>
/// Starts the frame.
/// </summary>
void ProfilerSystem::startFrame()
{
    currentFrameTime = SDL_GetTicks();
}

/// <summary>
/// Stores the sample.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="elapsedTime">The elapsed time.</param>
void ProfilerSystem::storeSample(const char* name, __int64 elapsedTime)
{
    SampleData* sample = new SampleData();
    sample->frameReference = currentFrameTime;
    sample->functionTime = elapsedTime;

    frameData[name].push_back(sample);
}

/// <summary>
/// Ends the frame.
/// </summary>
void ProfilerSystem::endFrame()
{
    __int64 frameTime = SDL_GetTicks() - currentFrameTime;
    float fps = 0.0f;
    if (frameTime > 0) {
        fps = 1000.0f / frameTime;
    }
    ImGui::Begin("FPS Counter");
    ImGui::Text("FPS: %.2f", fps);
    ImGui::End();
}




/// <summary>
/// The g profile manager
/// </summary>
ProfilerSystem g_profileManager;
