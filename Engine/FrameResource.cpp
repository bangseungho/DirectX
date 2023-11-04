#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(mCmdAlloc.GetAddressOf())));

    mPassCB = std::make_shared<UploadBuffer<PassConstants>>(device, 1, true);
    mMatData = std::make_shared<UploadBuffer<MaterialData>>(device, materialCount, false);
    mObjectCB = std::make_shared<UploadBuffer<ObjectData>>(device, objectCount, true);
}

FrameResource::~FrameResource()
{

}
