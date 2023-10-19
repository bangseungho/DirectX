#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CmdAlloc.GetAddressOf())));

    PassCB = std::make_shared<ConstantBuffer>();
    PassCB->Init(CBV_REGISTER::b0, sizeof(PassConstants), 1);

    ObjectCB = std::make_shared<ConstantBuffer>();
    ObjectCB->Init(CBV_REGISTER::b1, sizeof(ObjectConstants), objectCount);

    MaterialCB = std::make_shared<ConstantBuffer>();
    MaterialCB->Init(CBV_REGISTER::b2, sizeof(MaterialConstants), materialCount);
}

FrameResource::~FrameResource()
{

}

sptr<ConstantBuffer> FrameResource::GetConstantBuffer(CONSTANT_BUFFER_TYPE type)
{
    switch (type) {
    case CONSTANT_BUFFER_TYPE::PASS:
        return PassCB;
    case CONSTANT_BUFFER_TYPE::OBJECT:
        return ObjectCB;
    case CONSTANT_BUFFER_TYPE::MATERIAL:
        return MaterialCB;
    default:
        return nullptr;
    }
}
