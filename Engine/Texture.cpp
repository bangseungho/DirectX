#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

void Texture::Init(const wstring& path, TEXTURE_TYPE textureType)
{
	CreateTexture(path);
	CreateView(textureType);
}

void Texture::CreateTexture(const wstring& path)
{
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(DEVICE.Get(), CMD_LIST.Get(), path.c_str(), _resource, _uploadHeap));
}

void Texture::CreateView(TEXTURE_TYPE textureType)
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _resource->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	
	switch (textureType)
	{
	case TEXTURE_TYPE::TEXTURE2D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		break;
	case TEXTURE_TYPE::TEXTURECUBE:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		break;
	default:
		break;
	}

	DEVICE->CreateShaderResourceView(_resource.Get(), &srvDesc, _srvHandle);
}
