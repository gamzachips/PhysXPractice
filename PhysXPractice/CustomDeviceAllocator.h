#pragma once

class CustomDeviceAllocator : public PxDeviceAllocatorCallback {
public:
    // 메모리 할당
    virtual bool memAlloc(void** ptr, size_t size)
    {
        *ptr = malloc(size);
        return true;
    }

    // 메모리 해제
    virtual bool memFree(void* ptr)
    {
        free(ptr);
        return true;
    }
};