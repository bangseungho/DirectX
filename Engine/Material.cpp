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
	MATERIAL_CB->CopyData(_matCBIndex, _params);

	_shader->Update();
}

void Material::PushComputeData()
{
	_shader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	PushComputeData();

	ID3D12DescriptorHeap* descHeap = gEngine->GetTableDescHeap()->GetUAV().Get();
	COMPUTE_CMD_LIST->SetDescriptorHeaps(1, &descHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
	COMPUTE_CMD_LIST->SetComputeRootDescriptorTable(0, handle);

	COMPUTE_CMD_LIST->Dispatch(x, y, z);

	gEngine->GetComputeCmdQueue()->FlushComputeCommandQueue();
}