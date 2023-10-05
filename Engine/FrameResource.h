#pragma once
#include "ConstantBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device);
    ~FrameResource();

public:
    ComPtr<ID3D12CommandAllocator> _cmdAlloc;

    uptr<ConstantBuffer> ObjectCB = nullptr;

    uint64 Fence = 0;
};

