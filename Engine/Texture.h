#pragma once
#include "Object.h"

enum class TEXTURE_TYPE
{
	TEXTURE2D,
	TEXTURECUBE,
};

enum class RENDER_GROUP_TYPE
{
	SWAP_CHAIN,
	DEPTH_STENCIL,
	G_BUFFER
};

class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();

public:
	virtual void Load(const wstring& path);
	void Create(DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& property,
		D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor = Vec4());
	void CreateFromResource(ComPtr<ID3D12Resource> resource, RENDER_GROUP_TYPE groupType);
	void CreateSRVFromDescHeap(TEXTURE_TYPE type = TEXTURE_TYPE::TEXTURE2D);

public:
	ComPtr<ID3D12Resource> GetResource() { return _resource; }
	ComPtr<ID3D12DescriptorHeap> GetSRV() { return _srvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetRTV() { return _rtvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetDSV() { return _dsvHeap; }

	TEXTURE_TYPE GetTextureType() const { return _type; }

	void SetTexHeapIndex(uint32 index) { _texHeapIndex = index; }
	uint32 GetTexHeapIndex() const { return _texHeapIndex; }

private:
	ComPtr<ID3D12Resource>			_resource;
	ComPtr<ID3D12Resource>			_uploadHeap;
	TEXTURE_TYPE					_type;
	uint32							_texHeapIndex = 0;

	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE	_srvHeapBegin = {};

public:
	static uint32 TexHeapIndex;
};