#include "pch.h"
#include "DescriptorHeap.h"
#include "Engine.h"

void DescriptorHeap::Init()
{
	_cbvSrvUavDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// SRV Heap
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = TEXTURE_COUNT + COMPUTE_TEXTURE_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mSrvHeap));
	mSrvHeapBegin = mSrvHeap->GetCPUDescriptorHandleForHeapStart();

	// UAV Heap
	desc.NumDescriptors = COMPUTE_TEXTURE_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_uavHeap));
	_uavHeapBegin = _uavHeap->GetCPUDescriptorHandleForHeapStart();
}
