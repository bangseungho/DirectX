#include "pch.h"
#include "Texture.h"
#include "Engine.h"

uint32 Texture::TexHeapIndex = 0;

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS")
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(DEVICE.Get(), CMD_LIST.Get(), path.c_str(), _resource, _uploadHeap));
}

void Texture::Create(DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& property, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	desc.Flags = resFlags;

	D3D12_CLEAR_VALUE optimizedClearValue = {};
	D3D12_RESOURCE_STATES resStates = D3D12_RESOURCE_STATE_COMMON;

	if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
		resStates = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.f, 0);
	}
	else if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) {
		resStates = D3D12_RESOURCE_STATE_COMMON;
		float arrFloat[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
		optimizedClearValue = CD3DX12_CLEAR_VALUE(format, arrFloat);
	}

	ThrowIfFailed(DEVICE->CreateCommittedResource(
		&property,
		heapFlags,
		&desc,
		resStates,
		&optimizedClearValue,
		IID_PPV_ARGS(&_resource)
	));

	CreateFromResource(_resource, groupType);
}

void Texture::CreateFromResource(ComPtr<ID3D12Resource> resource, RENDER_GROUP_TYPE groupType)
{
	_resource = resource;
	
	D3D12_RESOURCE_DESC desc = _resource->GetDesc();

	// DSV
	if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
		DEVICE->CreateDepthStencilView(_resource.Get(), nullptr, dsvHandle);
	}
	else
	{
		if (desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			// RTV
			D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
			heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			heapDesc.NumDescriptors = 1;
			heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			heapDesc.NodeMask = 0;
			DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeap));

			D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
			DEVICE->CreateRenderTargetView(resource.Get(), nullptr, rtvHeapBegin);
		}

		switch (groupType)
		{
		case RENDER_GROUP_TYPE::SWAP_CHAIN:
		{
			// SRV
			D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
			srvHeapDesc.NumDescriptors = 1;
			srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

			_srvHeapBegin = _srvHeap->GetCPUDescriptorHandleForHeapStart();

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = resource->GetDesc().Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			DEVICE->CreateShaderResourceView(resource.Get(), &srvDesc, _srvHeapBegin);
		}
			break;
		case RENDER_GROUP_TYPE::G_BUFFER:
			CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURE2D);
			break;
		default:
			break;
		}


	}
}

void Texture::CreateSRVFromDescHeap(TEXTURE_TYPE type)
{
	_type = type;
	_texHeapIndex = TexHeapIndex++;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _resource->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	switch (type)
	{
	case TEXTURE_TYPE::TEXTURE2D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		break;
	case TEXTURE_TYPE::TEXTURECUBE:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = _resource->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
		DESCHEAP->SetSkyTexHeapIndex(_texHeapIndex);
		break;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHeapBegin = CD3DX12_CPU_DESCRIPTOR_HANDLE(DESCHEAP->GetSRVHandle());
	srvHeapBegin.Offset(_texHeapIndex, DESCHEAP->GetCbvSrvDescriptorSize());

	DEVICE->CreateShaderResourceView(_resource.Get(), &srvDesc, srvHeapBegin);
}
