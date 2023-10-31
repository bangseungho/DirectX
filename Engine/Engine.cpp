#include "pch.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Resources.h"

Engine::~Engine()
{
	if (_device != nullptr)
		_graphicsCmdQueue->WaitSync();
}

void Engine::Init(const WindowInfo& info)
{
	_window = info;

	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device = make_shared<Device>();
	_graphicsCmdQueue = make_shared<GraphicsCommandQueue>();
	_computeCmdQueue = make_shared<ComputeCommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();

	_device->Init();
	_graphicsCmdQueue->Init(_device->GetDevice(), _swapChain);
	_computeCmdQueue->Init(_device->GetDevice());
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _graphicsCmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init();

	CreateMultipleRenderTarget();

	ResizeWindow(info.width, info.height);

	GET_SINGLE(InputManager)->Init(info);
	GET_SINGLE(Timer)->Init(info);
	GET_SINGLE(Resources)->Init();
}

void Engine::Update()
{
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % FRAME_RESOURCE_COUNT;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	if (mCurrFrameResource->Fence != 0 && _graphicsCmdQueue->GetFence()->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(_graphicsCmdQueue->GetFence()->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	Render();
}

void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Render();

	RenderEnd();
}

void Engine::BuildFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount)
{
	for (int i = 0; i < FRAME_RESOURCE_COUNT; ++i)
	{
		mFrameResources[i] = std::make_unique<FrameResource>(device, objectCount, TEXTURE_COUNT);
	}
}

void Engine::RenderBegin()
{
	_graphicsCmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_graphicsCmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}

void Engine::CreateMultipleRenderTarget()
{
	// DepthStencil
	sptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture("DepthStencil",
		DXGI_FORMAT_D32_FLOAT, _window.width, _window.height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::DEPTH_STENCIL,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// SwapChain Group
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);

		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			string name = "SwapChainTarget_" + std::to_string(i);

			ComPtr<ID3D12Resource> resource;
			_swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resource, RENDER_GROUP_TYPE::SWAP_CHAIN);
		}

		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<MultipleRenderTarget>();
		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture("PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height, 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture("NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture("DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[3].target = GET_SINGLE(Resources)->CreateTexture("FresnelTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[4].target = GET_SINGLE(Resources)->CreateTexture("ShininessTarget",
			DXGI_FORMAT_R8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<MultipleRenderTarget>();
		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	// Lighting Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture("DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::LIGHTING,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture("SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::LIGHTING,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<MultipleRenderTarget>();
		_mrt[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}
}
