#pragma once

class CustomDeviceAllocator : public PxDeviceAllocatorCallback {
public:
    // �޸� �Ҵ�
    virtual bool memAlloc(void** ptr, size_t size)
    {
        *ptr = malloc(size);
        return true;
    }

    // �޸� ����
    virtual bool memFree(void* ptr)
    {
        free(ptr);
        return true;
    }
};