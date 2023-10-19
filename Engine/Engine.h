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
#include "DepthStencilBuffer.h"
#include "InputManager.h"
#include "Timer.h"

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
	sptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	sptr<SwapChain> GetSwapChain() { return _swapChain; }
	sptr<RootSignature> GetRootSignature() { return _rootSignature; }
	sptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	sptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }
	FrameResource* GetCurrFrameResource() { return	mCurrFrameResource; }
	WindowInfo GetWindow() const { return _window; }

public:
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	// window size
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	sptr<Device> _device;
	sptr<CommandQueue> _cmdQueue;
	sptr<SwapChain> _swapChain;
	sptr<RootSignature> _rootSignature;
	sptr<TableDescriptorHeap> _tableDescHeap;
	sptr<DepthStencilBuffer> _depthStencilBuffer;

	std::array<uptr<FrameResource>, FRAME_RESOURCE_COUNT> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	int mCurrFrameResourceIndex = 0;
};

