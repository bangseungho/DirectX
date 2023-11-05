#pragma once
#include "UploadBuffer.h"

class FrameResource
{
public:
    FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);
    ~FrameResource();

public:
    uint64                                   mFence = 0;

    ComPtr<ID3D12CommandAllocator>           mGraphicsCmdAlloc;

    sptr<UploadBuffer<PassConstants>>        mPassCB;
    sptr<UploadBuffer<ObjectData>>           mObjectCB;
    sptr<UploadBuffer<MaterialData>>         mMatData;
};

class ComputeFrameResource
{
public:
    ComputeFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);
    ~ComputeFrameResource();

public:
    uint64                                   mFence = 0;

    ComPtr<ID3D12CommandAllocator>           mComputeCmdAlloc;

    sptr<UploadBuffer<ParticleSystemData>>   mParticleSystemData;
};
