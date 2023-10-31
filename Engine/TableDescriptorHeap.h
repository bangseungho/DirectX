#pragma once
#include "Texture.h"

class TableDescriptorHeap
{
public:
	void Init();

	ComPtr<ID3D12DescriptorHeap> GetSRV() { return _srvHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return _srvHeapBegin; }

	ComPtr<ID3D12DescriptorHeap> GetUAV() { return _uavHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() { return _uavHeapBegin; }

	uint32 GetSkyTexHeapIndex() const { return _skyTexHeapIndex; }
	void SetSkyTexHeapIndex(uint32 index) { _skyTexHeapIndex = index; }
	uint32 GetCbvSrvUavDescriptorSize() { return _cbvSrvUavDescriptorSize; }

	uint32 AddSrvCount() { uint32 srvCount = _srvCount++; return srvCount; }
	uint32 AddUavCount() { uint32 uavCount = _uavCount++; return uavCount; }

	//void Update(uint8 currBackIndex);

private:
	ComPtr<ID3D12DescriptorHeap> _srvHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _srvHeapBegin = {};

	ComPtr<ID3D12DescriptorHeap> _uavHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _uavHeapBegin = {};

	uint32 _cbvSrvUavDescriptorSize;
	uint32 _skyTexHeapIndex;

	uint32 _srvCount = 0;
	uint32 _uavCount = 0;
};
