#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(mGraphicsCmdAlloc.GetAddressOf())));

    mPassCB = std::make_shared<UploadBuffer<PassConstants>>(device, 1, true);
    mObjectCB = std::make_shared<UploadBuffer<ObjectData>>(device, objectCount, true);
    mMatData = std::make_shared<UploadBuffer<MaterialData>>(device, materialCount, false);
    mTerrainData = std::make_shared<UploadBuffer<TerrainData>>(device, 1, false);
}

FrameResource::~FrameResource()
{

}

ComputeFrameResource::ComputeFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_COMPUTE,
        IID_PPV_ARGS(mComputeCmdAlloc.GetAddressOf())));

    mParticleSystemData = std::make_shared<UploadBuffer<ParticleSystemData>>(device, 1, false);
}

ComputeFrameResource::~ComputeFrameResource()
{
}
