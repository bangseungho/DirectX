#pragma once
#include "Texture.h"

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

