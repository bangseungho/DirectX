#pragma once
#include "UploadBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);
    ~FrameResource();

public:
    uint64                                   mFence = 0;
    uint64                                   mComputeFence = 0;

    ComPtr<ID3D12CommandAllocator>           mGraphicsCmdAlloc;
    ComPtr<ID3D12CommandAllocator>           mComputeCmdAlloc;

    sptr<UploadBuffer<PassConstants>>        mPassCB;
    sptr<UploadBuffer<ObjectData>>           mObjectCB;
    sptr<UploadBuffer<MaterialData>>         mMatData;
    sptr<UploadBuffer<ParticleSystemData>>   mParticleSystemData;
};
