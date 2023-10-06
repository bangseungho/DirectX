#include "pch.h"
#include "FrameResource.h"

FrameResource::FrameResource(ComPtr<ID3D12Device> device)
{
    ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CmdAlloc.GetAddressOf())));

    ObjectCB = std::make_shared<ConstantBuffer>();
    ObjectCB->Init(CBV_REGISTER::b0, sizeof(ObjectConstants), 256);

    MaterialCB = std::make_shared<ConstantBuffer>();
    MaterialCB->Init(CBV_REGISTER::b1, sizeof(MaterialConstants), 256);
}

FrameResource::~FrameResource()
{

}

sptr<ConstantBuffer> FrameResource::GetConstantBuffer(CONSTANT_BUFFER_TYPE type)
{
    switch (type) {
    case CONSTANT_BUFFER_TYPE::OBJECT:
        return ObjectCB;
    case CONSTANT_BUFFER_TYPE::MATERIAL:
        return MaterialCB;
    }
}
