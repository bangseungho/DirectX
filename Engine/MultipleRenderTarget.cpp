#include "pch.h"
#include "MultipleRenderTarget.h"
#include "Engine.h"
#include "Device.h"

void MultipleRenderTarget::Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, sptr<Texture> dsTexture)
{
	mGroupType = groupType;
	mRtVec = rtVec;
	mRtCount = static_cast<uint32>(rtVec.size());
	mDsTexture = dsTexture;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = mRtCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRtvHeap));

	_rtvHeapSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_rtvHeapBegin = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	_dsvHeapBegin = mDsTexture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	for (uint32 i = 0; i < mRtCount; ++i) {
		uint32 destSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, i * _rtvHeapSize);
		
		uint32 srcSize = 1;
		ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = mRtVec[i].target->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

		DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	for (uint32 i = 0; i < mRtCount; ++i) {
		_targetToResource[i] = CD3DX12_RESOURCE_BARRIER::Transition(mRtVec[i].target->GetResource().Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);

		_resourceToTarget[i] = CD3DX12_RESOURCE_BARRIER::Transition(mRtVec[i].target->GetResource().Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void MultipleRenderTarget::OMSetRenderTargets(uint32 count, uint32 offset)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, offset * _rtvHeapSize);
	GRAPHICS_CMD_LIST->OMSetRenderTargets(count, &rtvHandle, FALSE, &_dsvHeapBegin);
}

void MultipleRenderTarget::OMSetRenderTargets()
{
	GRAPHICS_CMD_LIST->OMSetRenderTargets(mRtCount, &_rtvHeapBegin, TRUE, &_dsvHeapBegin);
}

void MultipleRenderTarget::ClearRenderTargetView(uint32 index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, index * _rtvHeapSize);
	GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, mRtVec[index].clearColor, 0, nullptr);

	GRAPHICS_CMD_LIST->ClearDepthStencilView(_dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void MultipleRenderTarget::ClearRenderTargetView()
{
	WaitResourceToTarget();

	for (uint32 i = 0; i < mRtCount; ++i) {
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, i * _rtvHeapSize);
		GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, mRtVec[i].clearColor, 0, nullptr);
	}
}

void MultipleRenderTarget::ClearDepthStencilView()
{
	GRAPHICS_CMD_LIST->ClearDepthStencilView(_dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void MultipleRenderTarget::WaitTargetToResource()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(mRtCount, _targetToResource);
}

void MultipleRenderTarget::WaitResourceToTarget()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(mRtCount, _resourceToTarget);
}
