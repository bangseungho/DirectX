#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CmdAlloc.GetAddressOf())));

    PassCB = std::make_shared<UploadBuffer<PassConstants>>(device, 1, true);
    MaterialCB = std::make_shared<UploadBuffer<MaterialConstants>>(device, materialCount, false);
    ObjectCB = std::make_shared<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}

FrameResource::~FrameResource()
{

}
