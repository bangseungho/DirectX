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
	//CB(CONSTANT_BUFFER_TYPE::MATERIAL)->CopyData(_matCBIndex, &_params, sizeof(MaterialConstants));

	MATERIAL_CB->CopyData(_matCBIndex, _params);

	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(DESCHEAP->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(_diffuseSrvHeapIndex, DESCHEAP->GetCbvSrvDescriptorSize());
	CMD_LIST->SetGraphicsRootDescriptorTable(3, tex);

	_shader->Update();
}

