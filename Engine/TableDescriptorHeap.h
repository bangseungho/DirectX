#pragma once


class TableDescriptorHeap
{
public:
	void Init(uint32 count);

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }
	uint32 GetCbvSrvDescriptorSize() { return _cbvSrvDescriptorSize; }

	void CreateSRV(sptr<class Texture> texture);

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor;
	uint32 _cbvSrvDescriptorSize;
};

