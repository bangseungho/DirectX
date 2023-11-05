#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "DescriptorHeap.h"
#include "FrameResource.h"
#include "Texture.h"
#include "InputManager.h"
#include "Timer.h"
#include "MultipleRenderTarget.h"

class Engine
{
public:
	~Engine();

public:
	void Init(const WindowInfo& info);
	void Update();
	void Render();
	void BuildFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount);

public:
	sptr<Device> GetDevice() { return mDevice; }
	sptr<GraphicsCommandqueue> GetGraphicsCmdQueue() { return mGraphicsCmdQueue; }
	sptr<ComputeCommandQueue> GetComputeCmdQueue() { return mComputeCmdQueue; }
	sptr<SwapChain> GetSwapChain() { return mSwapChain; }
	sptr<RootSignature> GetRootSignature() { return mRootSignature; }
	sptr<DescriptorHeap> GetTableDescHeap() { return mDescHeap; }
	sptr<MultipleRenderTarget> GetMRT(RENDER_TARGET_GROUP_TYPE type) { return mMrtGroup[static_cast<uint8>(type)]; }
	FrameResource* GetCurrFrameResource() { return	mCurrFrameResource; }
	ComputeFrameResource* GetCurrComputeFrameResource() { return mCurrComputeFrameResource; }
	WindowInfo GetWindow() const { return mWindow; }

public:
	void RenderBegin();
	void RenderEnd();
	void ResizeWindow(int32 Width, int32 Height);

private:
	void CreateMultipleRenderTarget();

private:
	WindowInfo						mWindow = {};
	D3D12_VIEWPORT					mViewport = {};
	D3D12_RECT						mScissorRect = {};

	sptr<Device>					mDevice = make_shared<Device>();
	sptr<GraphicsCommandqueue>		mGraphicsCmdQueue = make_shared<GraphicsCommandqueue>();
	sptr<ComputeCommandQueue>		mComputeCmdQueue = make_shared<ComputeCommandQueue>();
	sptr<SwapChain>					mSwapChain = make_shared<SwapChain>();
	sptr<RootSignature>				mRootSignature = make_shared<RootSignature>();
	sptr<DescriptorHeap>			mDescHeap = make_shared<DescriptorHeap>();

	array<sptr<MultipleRenderTarget>, RENDER_TARGET_GROUP_COUNT> mMrtGroup;

	std::array<uptr<FrameResource>, FRAME_RESOURCE_COUNT> mFrameResources;
	std::array<uptr<ComputeFrameResource>, FRAME_RESOURCE_COUNT> mComputeFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	ComputeFrameResource* mCurrComputeFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
	int mCurrComputeFrameResourceIndex = 0;
};

