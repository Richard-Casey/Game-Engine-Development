#pragma once

#include <map>
#include <iostream>
#include <vector>
#include "Game.h"
#include <Windows.h>
#include <string>


struct SampleData
{
    /// <summary>
    /// The function time
    /// </summary>
    __int64 functionTime;
    /// <summary>
    /// The frame reference
    /// </summary>
    int frameReference;
};

typedef std::map<const char*, std::vector<SampleData*>> FrameMap;

class ProfilerSystem
{
public:
    ProfilerSystem();
    ~ProfilerSystem();

    void startFrame();
    void storeSample(const char* name, __int64 elapsedTime);
    void endFrame();

    __int64 currentFrameTime = 0;

private:

    /// <summary>
    /// The current frame
    /// </summary>
    int currentFrame;
    /// <summary>
    /// The frame data
    /// </summary>
    FrameMap frameData;
    int frameCount;
    float totalFrameTime;
};


extern ProfilerSystem g_profileManager;


struct Profile
{
    /// <summary>
    /// Initializes a new instance of the <see cref="Profile"/> struct.
    /// </summary>
    /// <param name="name">The name.</param>
    Profile(const char* name)
    {
        _name = name;
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        startTime = li.QuadPart;
    }

    /// <summary>
    /// Finalizes an instance of the <see cref="Profile"/> class.
    /// </summary>
    ~Profile()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        __int64 endTime = li.QuadPart;
        __int64 elapsedTime = endTime - startTime;
        g_profileManager.storeSample(_name, elapsedTime);
    }

    /// <summary>
    /// The name
    /// </summary>
    const char* _name;
    /// <summary>
    /// The start time
    /// </summary>
    __int64 startTime;
};
#define PROFILE(name) Profile p(name)