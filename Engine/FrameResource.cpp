#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(_cmdAlloc.GetAddressOf())));

    ObjectCB = std::make_unique<ConstantBuffer>();
    ObjectCB->Init(sizeof(ObjectConstants), 256);
}

FrameResource::~FrameResource()
{

}