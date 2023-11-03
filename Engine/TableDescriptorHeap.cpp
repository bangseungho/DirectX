#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init()
{
	_cbvSrvUavDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// SRV Heap
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = TEXTURE_COUNT + COMPUTE_TEXTURE_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvHeap));
	_srvHeapBegin = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	// UAV Heap
	desc.NumDescriptors = COMPUTE_TEXTURE_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_uavHeap));
	_uavHeapBegin = _uavHeap->GetCPUDescriptorHandleForHeapStart();
}
