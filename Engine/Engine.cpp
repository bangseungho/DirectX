#include "pch.h"
#include "Engine.h"
#include "SceneManager.h"

Engine::~Engine()
{
	if (_device != nullptr)
		_cmdQueue->WaitSync();
}

void Engine::Init(const WindowInfo& info)
{
	_window = info;

	GET_SINGLE(InputManager)->Init(info);
	GET_SINGLE(Timer)->Init(info);

	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();
	_depthStencilBuffer = make_shared<DepthStencilBuffer>();

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init(_device->GetDevice());
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);

	ResizeWindow(info.width, info.height);

	_cmdQueue->BuildFrameResource(_device->GetDevice());
}

void Engine::Update()
{
	_cmdQueue->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(Timer)->Update();

	Render();
}

void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Update();

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	_depthStencilBuffer->Init(_window);
}
