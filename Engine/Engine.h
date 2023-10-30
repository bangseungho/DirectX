#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "TableDescriptorHeap.h"
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
	void BuildFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount);

public:
	sptr<Device> GetDevice() { return _device; }
	sptr<GraphicsCommandQueue> GetCmdQueue() { return _cmdQueue; }
	sptr<SwapChain> GetSwapChain() { return _swapChain; }
	sptr<RootSignature> GetRootSignature() { return _rootSignature; }
	sptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	sptr<MultipleRenderTarget> GetMRT(RENDER_TARGET_GROUP_TYPE type) { return _mrt[static_cast<uint8>(type)]; }

	FrameResource* GetCurrFrameResource() { return	mCurrFrameResource; }
	WindowInfo GetWindow() const { return _window; }

public:
	void RenderBegin();
	void RenderEnd();
	void ResizeWindow(int32 width, int32 height);

private:
	void CreateMultipleRenderTarget();

private:
	// window size
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	sptr<Device> _device;
	sptr<GraphicsCommandQueue> _cmdQueue;
	sptr<SwapChain> _swapChain;
	sptr<RootSignature> _rootSignature;
	sptr<TableDescriptorHeap> _tableDescHeap;
	array<sptr<MultipleRenderTarget>, RENDER_TARGET_GROUP_COUNT> _mrt;

	std::array<uptr<FrameResource>, FRAME_RESOURCE_COUNT> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
};

