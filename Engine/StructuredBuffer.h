#pragma once

class StructuredBuffer
{
public:
	StructuredBuffer();
	~StructuredBuffer();

	void Init(uint32 elementSize, uint32 elementCount);

	void PushComputeData(UAV_REGISTER reg);
	void PushGraphicsData(uint8 reg);

	ComPtr<ID3D12DescriptorHeap> GetSRV() { return mSrvHeap; }
	ComPtr<ID3D12DescriptorHeap> GetUAV() { return mUavHeap; }

	void SetResourceState(D3D12_RESOURCE_STATES state) { mResourceState = state; }
	D3D12_RESOURCE_STATES GetResourceState() { return mResourceState; }
	ComPtr<ID3D12Resource> GetBuffer() { return mBuffer; }

private:
	ComPtr<ID3D12Resource>			mBuffer;
	ComPtr<ID3D12DescriptorHeap>	mSrvHeap;
	ComPtr<ID3D12DescriptorHeap>	mUavHeap;

	uint32						mElementSize = 0;
	uint32						mElementCount = 0;
	D3D12_RESOURCE_STATES		mResourceState = {};

private:
	D3D12_CPU_DESCRIPTOR_HANDLE mSrvHeapBegin = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mUavHeapBegin = {};
};

