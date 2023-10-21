#pragma once
#include "UploadBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);
    ~FrameResource();

public:
    ComPtr<ID3D12CommandAllocator> CmdAlloc;

    sptr<UploadBuffer<PassConstants>> PassCB;
    sptr<UploadBuffer<ObjectConstants>> ObjectCB;
    sptr<UploadBuffer<MaterialConstants>> MaterialCB;

    uint64 Fence = 0;
};
