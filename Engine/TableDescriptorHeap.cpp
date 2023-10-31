#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init()
{
	_cbvSrvUavDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// SRV Heap
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = TEXTURE_COUNT + 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvHeap));
	_srvHeapBegin = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	// UAV Heap
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_uavHeap));
	_uavHeapBegin = _uavHeap->GetCPUDescriptorHandleForHeapStart();
}

//void TableDescriptorHeap::Update(uint8 currBackIndex)
//{
//	CD3DX12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_srvHeap->GetCPUDescriptorHandleForHeapStart());
//	destHandle.Offset(Texture::TexHeapIndex, _cbvSrvDescriptorSize);
//
//	auto srcHandle = gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(currBackIndex)->GetSRVHandle();
//	uint32 destSize = 1;
//	uint32 srcsize = 1;
//
//	DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcsize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//}
