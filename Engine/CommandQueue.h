#pragma once

class SwapChain;
class DescriptorHeap;
class FrameResource;

class GraphicsCommandqueue
{
public:
	~GraphicsCommandqueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();
	void FlushResourceCommandQueue();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return mCmdQueue; }

	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	mCmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResCmdList() { return mResCmdList; }

	ComPtr<ID3D12CommandAllocator> GetCmdAlloc() { return	mGraphicsCmdAlloc; }
	ComPtr<ID3D12Fence> GetFence() { return	mFence; }

private:
	ComPtr<ID3D12CommandQueue>			mCmdQueue;

	ComPtr<ID3D12CommandAllocator>		mGraphicsCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	mCmdList;

	ComPtr<ID3D12CommandAllocator>		mResCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	mResCmdList;

	ComPtr<ID3D12Fence>					mFence;
	uint32								mFenceValue = 0;
	HANDLE								mFenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>				mSwapChain;

};

class ComputeCommandQueue
{
public:
	~ComputeCommandQueue();

	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();

	void RenderBegin();
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return mCmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return mCmdList; }
	ComPtr<ID3D12CommandAllocator> GetCmdAlloc() { return mCmdAlloc; }

	ComPtr<ID3D12Fence> GetFence() { return	mFence; }

private:
	ComPtr<ID3D12CommandQueue>			mCmdQueue;
	ComPtr<ID3D12CommandAllocator>		mCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	mCmdList;

	ComPtr<ID3D12Fence>					mFence;
	uint32								mFenceValue = 0;
	HANDLE								mFenceEvent = INVALID_HANDLE_VALUE;
};
