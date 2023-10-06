#pragma once
#include "ConstantBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device);
    ~FrameResource();

    sptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type);

public:
    ComPtr<ID3D12CommandAllocator> CmdAlloc;

    sptr<ConstantBuffer> ObjectCB = nullptr;
    sptr<ConstantBuffer> MaterialCB = nullptr;

    uint64 Fence = 0;
};

