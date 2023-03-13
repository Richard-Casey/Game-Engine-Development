#pragma once

#include <map>
#include <iostream>
#include <vector>
#include "Game.h"
#include <Windows.h>
#include <string>





struct SampleData
{
    __int64 functionTime;
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

private:

    int currentFrame;
    FrameMap frameData;
};


extern ProfilerSystem g_profileManager;


struct Profile
{
    Profile(const char* name)
    {
        _name = name;
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        startTime = li.QuadPart;
    }

    ~Profile()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        __int64 endTime = li.QuadPart;
        __int64 elapsedTime = endTime - startTime;
        g_profileManager.storeSample(_name, elapsedTime);
    }

    const char* _name;
    __int64 startTime;
};
#define PROFILE(name) Profile p(name)