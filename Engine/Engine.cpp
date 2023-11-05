#include "pch.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Resources.h"

Engine::~Engine()
{
	if (mDevice != nullptr)
		mGraphicsCmdQueue->WaitSync();
}

void Engine::Init(const WindowInfo& info)
{
	mWindow = info;

	mViewport = { 0, 0, static_cast<FLOAT>(info.Width), static_cast<FLOAT>(info.Height), 0.0f, 1.0f };
	mScissorRect = CD3DX12_RECT(0, 0, info.Width, info.Height);

	mDevice->Init();
	mGraphicsCmdQueue->Init(mDevice->GetDevice(), mSwapChain);
	mComputeCmdQueue->Init(mDevice->GetDevice());
	mSwapChain->Init(info, mDevice->GetDevice(), mDevice->GetDXGI(), mGraphicsCmdQueue->GetCmdQueue());
	mRootSignature->Init();
	mDescHeap->Init();

	CreateMultipleRenderTarget();

	ResizeWindow(info.Width, info.Height);

	GET_SINGLE(InputManager)->Init(info);
	GET_SINGLE(Timer)->Init(info);
	GET_SINGLE(Resources)->Init();
}

void Engine::Update()
{
	mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % FRAME_RESOURCE_COUNT;
	mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

	if (mCurrFrameResource->mFence != 0 && mGraphicsCmdQueue->GetFence()->GetCompletedValue() < mCurrFrameResource->mFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mGraphicsCmdQueue->GetFence()->SetEventOnCompletion(mCurrFrameResource->mFence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	mCurrComputeFrameResourceIndex = (mCurrComputeFrameResourceIndex + 1) % FRAME_RESOURCE_COUNT;
	mCurrComputeFrameResource = mComputeFrameResources[mCurrComputeFrameResourceIndex].get();

	if (mCurrComputeFrameResource->mFence != 0 && mComputeCmdQueue->GetFence()->GetCompletedValue() < mCurrComputeFrameResource->mFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mComputeCmdQueue->GetFence()->SetEventOnCompletion(mCurrComputeFrameResource->mFence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	GET_SINGLE(SceneManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(Timer)->Update();

	Render();
}

void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Render();

	RenderEnd();
}

void Engine::BuildFrameResource(ComPtr<ID3D12Device> device, uint32 objectCount, uint32 materialCount)
{
	for (int i = 0; i < FRAME_RESOURCE_COUNT; ++i)
	{
		mFrameResources[i] = std::make_unique<FrameResource>(device, objectCount, materialCount);
		mComputeFrameResources[i] = std::make_unique<ComputeFrameResource>(device, 0, 0);
	}
}

void Engine::RenderBegin()
{
	mComputeCmdQueue->RenderBegin();
	mGraphicsCmdQueue->RenderBegin(&mViewport, &mScissorRect);
}

void Engine::RenderEnd()
{
	mComputeCmdQueue->RenderEnd();
	mGraphicsCmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 Width, int32 Height)
{
	mWindow.Width = Width;
	mWindow.Height = Height;

	RECT rect = { 0, 0, Width, Height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(mWindow.Hwnd, 0, 100, 100, Width, Height, 0);
}

void Engine::CreateMultipleRenderTarget()
{
	// DepthStencil
	sptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture("DepthStencil",
		DXGI_FORMAT_D32_FLOAT, mWindow.Width, mWindow.Height,
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
			mSwapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resource, RENDER_GROUP_TYPE::SWAP_CHAIN);
		}

		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<MultipleRenderTarget>();
		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture("PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, mWindow.Width, mWindow.Height, 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture("NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture("DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[3].target = GET_SINGLE(Resources)->CreateTexture("FresnelTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[4].target = GET_SINGLE(Resources)->CreateTexture("ShininessTarget",
			DXGI_FORMAT_R8_UNORM, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::G_BUFFER, 
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<MultipleRenderTarget>();
		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}

	// Lighting Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture("DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::LIGHTING,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture("SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, mWindow.Width, mWindow.Height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, RENDER_GROUP_TYPE::LIGHTING,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<MultipleRenderTarget>();
		mMrtGroup[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}
}
