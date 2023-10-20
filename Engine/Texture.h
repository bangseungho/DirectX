#pragma once
#include "Object.h"

enum class TEXTURE_TYPE
{
	TEXTURE2D,
	TEXTURECUBE,
};

class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();

public:
	void Init(const wstring& path);

	ComPtr<ID3D12Resource> Resource() { return _resource; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

public:
	void CreateTexture(const wstring& path);

private:
	ComPtr<ID3D12Resource>			_resource;
	ComPtr<ID3D12Resource>			_uploadHeap;

	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle;
};