#pragma once
#include "Texture.h"

class TableDescriptorHeap
{
public:
	void Init();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }
	uint32 GetCbvSrvDescriptorSize() { return _cbvSrvDescriptorSize; }

	void CreateSRV(sptr<class Texture> texture, TEXTURE_TYPE type = TEXTURE_TYPE::TEXTURE2D);
	uint32 GetSkyTexHeapIndex() const { return _skyTexHeapIndex; }

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor;
	uint32 _cbvSrvDescriptorSize;
	uint32 _skyTexHeapIndex;
};

