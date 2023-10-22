#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	G_BUFFER, // POSITION, NORMAL, DIFFUSEALBEDO, SHINESS, FRESNELR0
	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_COUNT = 1,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END)
};

struct RenderTarget
{
	sptr<Texture> target;
	float clearColor[4];
};

class MultipleRenderTarget
{
public:
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, sptr<Texture> dsTexture);

	void OMSetRenderTargets(uint32 count, uint32 offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(uint32 index);
	void ClearRenderTargetView();

	sptr<Texture> GetRTTexture(uint32 index) 
	{ 
		return _rtVec[index].target;
	}
	sptr<Texture> GetDSTexture() { return _dsTexture; }

private:
	RENDER_TARGET_GROUP_TYPE _groupType;
	vector<RenderTarget> _rtVec;
	uint32 _rtCount;
	sptr<Texture> _dsTexture;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;

	uint32 _rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE _dsvHeapBegin;
};

