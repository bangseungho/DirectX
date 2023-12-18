#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	wstring ext = fs::path(path).extension();

	if (ext == L".dds" || ext == L".DDS") {
		ThrowIfFailed(DirectX::CreateDDSTextureFromFile12(DEVICE.Get(), GRAPHICS_RES_CMD_LIST.Get(), path.c_str(), _resource, _uploadHeap));
	}
	else {
		Texture::LoadFromWICFile(path);
	}

	_desc = _resource->GetDesc();

	gEngine->GetGraphicsCmdQueue()->FlushResourceCommandQueue();

	CreateSRVFromDescHeap();
}

void Texture::LoadFromWICFile(const wstring& path)
{
	::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, mImage);

	HRESULT hr = ::CreateTexture(DEVICE.Get(), mImage.GetMetadata(), &_resource);

	if (FAILED(hr))
		assert(nullptr);

	std::vector<D3D12_SUBRESOURCE_DATA> subResources;

	ThrowIfFailed(PrepareUpload(DEVICE.Get(),
		mImage.GetImages(),
		mImage.GetImageCount(),
		mImage.GetMetadata(),
		subResources));

	const UINT64 bufferSize = ::GetRequiredIntermediateSize(_resource.Get(), 0, static_cast<UINT32>(subResources.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ThrowIfFailed(DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_uploadHeap)));

	::UpdateSubresources(GRAPHICS_RES_CMD_LIST.Get(),
		_resource.Get(),
		_uploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResources.size()),
		subResources.data());
}

void Texture::Create(DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& property, D3D12_HEAP_FLAGS heapFlags, RENDER_GROUP_TYPE groupType, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	_desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	_desc.Flags = resFlags;

	D3D12_CLEAR_VALUE optimizedClearValue = {};
	D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr;

	D3D12_RESOURCE_STATES resStates = D3D12_RESOURCE_STATE_COMMON;

	if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
		resStates = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		pOptimizedClearValue = &optimizedClearValue;
	}
	else if (resFlags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) {
		resStates = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		float arrFloat[4] = { clearColor.x, clearColor.y, clearColor.z, clearColor.w };
		optimizedClearValue = CD3DX12_CLEAR_VALUE(format, arrFloat);
		pOptimizedClearValue = &optimizedClearValue;
	}

	ThrowIfFailed(DEVICE->CreateCommittedResource(
		&property,
		heapFlags,
		&_desc,
		resStates,
		pOptimizedClearValue,
		IID_PPV_ARGS(&_resource)
	));

	CreateFromResource(_resource, groupType);
}

void Texture::CreateFromResource(ComPtr<ID3D12Resource> resource, RENDER_GROUP_TYPE groupType)
{
	_resource = resource;
	
	_desc = _resource->GetDesc();

	// DSV
	if (_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) {
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
		if (_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
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

		// UAV
		if (_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
		{
			CreateUAVFromDescHeap();
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
		case RENDER_GROUP_TYPE::LIGHTING:
		case RENDER_GROUP_TYPE::COMPUTE:
		case RENDER_GROUP_TYPE::CUBEMAP:
			CreateSRVFromDescHeap();
			break;
		default:
			break;
		}
	}
}

void Texture::CreateSRVFromDescHeap()
{
	_texHeapIndex = DESCHEAP->AddSrvCount();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _resource->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHeapBegin = CD3DX12_CPU_DESCRIPTOR_HANDLE(DESCHEAP->GetSRVHandle());
	srvHeapBegin.Offset(_texHeapIndex, DESCHEAP->GetCbvSrvUavDescriptorSize());

	DEVICE->CreateShaderResourceView(_resource.Get(), &srvDesc, srvHeapBegin);
}

void Texture::CreateUAVFromDescHeap()
{
	_uavHeapIndex = DESCHEAP->AddUavCount();

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = _resource->GetDesc().Format;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

	CD3DX12_CPU_DESCRIPTOR_HANDLE uavHeapBegin = CD3DX12_CPU_DESCRIPTOR_HANDLE(DESCHEAP->GetUAVHandle());
	uavHeapBegin.Offset(_uavHeapIndex, DESCHEAP->GetCbvSrvUavDescriptorSize());

	DEVICE->CreateUnorderedAccessView(_resource.Get(), nullptr, &uavDesc, uavHeapBegin);
}

TextureCube::TextureCube()
{
}

TextureCube::~TextureCube()
{
}

void TextureCube::CreateSRVFromDescHeap()
{
	_texHeapIndex = DESCHEAP->AddSrvCount();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _resource->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = _resource->GetDesc().MipLevels;
	srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
	DESCHEAP->SetSkyTexHeapIndex(_texHeapIndex);

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHeapBegin = CD3DX12_CPU_DESCRIPTOR_HANDLE(DESCHEAP->GetSRVHandle());
	srvHeapBegin.Offset(_texHeapIndex, DESCHEAP->GetCbvSrvUavDescriptorSize());

	DEVICE->CreateShaderResourceView(_resource.Get(), &srvDesc, srvHeapBegin);
}
