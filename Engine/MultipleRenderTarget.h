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
	void ClearDepthStencilView();

	sptr<Texture> GetRTTexture(uint32 index) { return mRtVec[index].target; }
	sptr<Texture> GetDSTexture() { return mDsTexture; }

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetRTVHandle() { return _rtvHeapBegin; }

	void WaitTargetToResource();
	void WaitResourceToTarget();

private:
	RENDER_TARGET_GROUP_TYPE		mGroupType;
	vector<RenderTarget>			mRtVec;
	uint32 mRtCount;
	sptr<Texture> mDsTexture;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;

	uint32 _rtvHeapSize;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _rtvHeapBegin;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _dsvHeapBegin;

	D3D12_RESOURCE_BARRIER _targetToResource[8];
	D3D12_RESOURCE_BARRIER _resourceToTarget[8];
};

