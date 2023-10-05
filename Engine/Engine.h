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

class Engine
{
public:
	~Engine();

public:
	void Init(const WindowInfo& info);
	void Update();
	void Render();

public:
	sptr<Device> GetDevice() { return _device; }
	sptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	sptr<SwapChain> GetSwapChain() { return _swapChain; }
	sptr<RootSignature> GetRootSignature() { return _rootSignature; }
	sptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	sptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }

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
};

