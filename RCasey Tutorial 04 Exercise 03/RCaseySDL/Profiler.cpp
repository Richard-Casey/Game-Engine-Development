#include "Profiler.h"
#include <iostream>
#include <vector>





/// <summary>
/// Initializes a new instance of the <see cref="ProfilerSystem"/> class.
/// </summary>
ProfilerSystem::ProfilerSystem()
{
}

/// <summary>
/// Finalizes an instance of the <see cref="ProfilerSystem"/> class.
/// </summary>
ProfilerSystem::~ProfilerSystem()
{
    
}

/// <summary>
/// Starts the frame.
/// </summary>
void ProfilerSystem::startFrame()
{
    //make a new frame sample
    //get current time SDL_ticks()
}

/// <summary>
/// Stores the sample.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="elapsedTime">The elapsed time.</param>
void ProfilerSystem::storeSample(const char* name, __int64 elapsedTime)
{
    SampleData* sample = new SampleData();
    sample->frameReference = currentFrame;
    sample->functionTime = elapsedTime;

    frameData[name].push_back(sample);
}

/// <summary>
/// Ends the frame.
/// </summary>
void ProfilerSystem::endFrame()
{
    //call store sample
}


/// <summary>
/// The g profile manager
/// </summary>
ProfilerSystem g_profileManager;
