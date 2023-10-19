#include "pch.h"
#include "Material.h"
#include "Engine.h"


Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
}

Material::~Material()
{
}

void Material::Update()
{
	CB(CONSTANT_BUFFER_TYPE::MATERIAL)->CopyData(_matCBIndex, &_params, sizeof(MaterialConstants));

	UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
	D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = CURR_FRAMERESOURCE->MaterialCB->Resource()->GetGPUVirtualAddress() + _matCBIndex * matCBByteSize;
	CMD_LIST->SetGraphicsRootConstantBufferView(2, matCBAddress);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(DESCHEAP->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(_diffuseSrvHeapIndex, DESCHEAP->GetCbvSrvDescriptorSize());
	CMD_LIST->SetGraphicsRootDescriptorTable(3, tex);

	_shader->Update();
}

