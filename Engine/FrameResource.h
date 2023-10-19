#pragma once
#include "ConstantBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);
    ~FrameResource();

    sptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type);

public:
    ComPtr<ID3D12CommandAllocator> CmdAlloc;

    sptr<ConstantBuffer> PassCB;
    sptr<ConstantBuffer> ObjectCB;
    sptr<ConstantBuffer> MaterialCB;

    uint64 Fence = 0;
};

