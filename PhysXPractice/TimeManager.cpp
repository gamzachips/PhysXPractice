#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
}

void TimeManager::Update()
{
    unsigned int currentCount;
    LARGE_INTEGER performanceCount;
    ::QueryPerformanceCounter(&performanceCount);
    currentCount = performanceCount.QuadPart;

    _deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
    _prevCount = currentCount;
}
