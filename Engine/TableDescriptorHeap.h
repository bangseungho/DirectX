#pragma once
#include "Texture.h"

class TableDescriptorHeap
{
public:
	void Init();

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return _srvHeapBegin; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return _srvHeap; }

	uint32 GetSkyTexHeapIndex() const { return _skyTexHeapIndex; }
	void SetSkyTexHeapIndex(uint32 index) { _skyTexHeapIndex = index; }

	uint32 GetCbvSrvDescriptorSize() { return _cbvSrvDescriptorSize; }

	void Update(uint8 currBackIndex);

private:
	ComPtr<ID3D12DescriptorHeap> _srvHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _srvHeapBegin = {};

	uint32 _cbvSrvDescriptorSize;
	uint32 _skyTexHeapIndex;
};

