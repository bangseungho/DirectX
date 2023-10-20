#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = TEXTURE_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_cbvSrvDescriptorSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	hDescriptor = _descHeap->GetCPUDescriptorHandleForHeapStart();
}

void TableDescriptorHeap::CreateSRV(sptr<Texture> texture, TEXTURE_TYPE type)
{
	auto tex = texture->Resource();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = tex->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	switch (type)
	{
	case TEXTURE_TYPE::TEXTURE2D:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		break;
	case TEXTURE_TYPE::TEXTURECUBE:
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = tex->GetDesc().MipLevels;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.f;
		_skyTexHeapIndex = static_cast<uint8>(TEXTURECUBE_INDEX::SKYBOX);
		break;
	}

	DEVICE->CreateShaderResourceView(tex.Get(), &srvDesc, hDescriptor);
	hDescriptor.Offset(1, _cbvSrvDescriptorSize);
}
