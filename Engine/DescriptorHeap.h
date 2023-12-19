#pragma once
#include "Texture.h"

class DescriptorHeap
{
public:
	void Init();

	ComPtr<ID3D12DescriptorHeap> GetSRV() { return mSrvHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return mSrvHeapBegin; }

	ComPtr<ID3D12DescriptorHeap> GetUAV() { return _uavHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() { return _uavHeapBegin; }

	uint32 GetSkyTexHeapIndex() const { return _skyTexHeapIndex; }
	void SetSkyTexHeapIndex(uint32 index) { _skyTexHeapIndex = index; }
	uint32 GetCbvSrvUavDescriptorSize() { return _cbvSrvUavDescriptorSize; }

	uint32 GetCubeMapTexHeapIndex() const { return mCubeMapTexHeapIndex; }
	void SetCubeMapTexHeapIndex(uint32 index) { mCubeMapTexHeapIndex = index; }

	uint32 AddSrvCount() { uint32 srvCount = _srvCount++; return srvCount; }
	uint32 AddUavCount() { uint32 uavCount = _uavCount++; return uavCount; }

private:
	ComPtr<ID3D12DescriptorHeap> mSrvHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mSrvHeapBegin = {};

	ComPtr<ID3D12DescriptorHeap> _uavHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _uavHeapBegin = {};

	uint32 _cbvSrvUavDescriptorSize;
	uint32 _skyTexHeapIndex;
	uint32 mCubeMapTexHeapIndex;

	uint32 _srvCount = 0;
	uint32 _uavCount = 0;
};
