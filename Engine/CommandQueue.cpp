#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"
#include "Material.h"
#include "Resources.h"

//===================================================================GraphicsCommandQueue

CommandQueue::~CommandQueue()
{
	::CloseHandle(mFenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	mSwapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCmdQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&mCmdList));

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	mFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	mFenceValue++;

	mCmdQueue->Signal(mFence.Get(), mFenceValue);

	if (mFence->GetCompletedValue() < mFenceValue)
	{
		mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);

		::WaitForSingleObject(mFenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	auto cmdAlloc = CURR_FRAMERESOURCE->mCmdAlloc;

	cmdAlloc->Reset();
	mCmdList->Reset(cmdAlloc.Get(), nullptr);

	int8 backIndex = mSwapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetResource().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	mCmdList->SetGraphicsRootSignature(GRAPHICS_ROOT_SIGNATURE.Get());

	ID3D12DescriptorHeap* descHeap = gEngine->GetTableDescHeap()->GetSRV().Get();
	mCmdList->SetDescriptorHeaps(1, &descHeap);

	auto passCB = CURR_FRAMERESOURCE->mPassCB->Resource();
	mCmdList->SetGraphicsRootConstantBufferView(0, passCB->GetGPUVirtualAddress());

	auto matData = MATERIAL_CB->Resource();
	mCmdList->SetGraphicsRootShaderResourceView(2, matData->GetGPUVirtualAddress());

	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(DESCHEAP->GetSRV()->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(DESCHEAP->GetSkyTexHeapIndex(), DESCHEAP->GetCbvSrvUavDescriptorSize());
	mCmdList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

	mCmdList->SetGraphicsRootDescriptorTable(4, DESCHEAP->GetSRV()->GetGPUDescriptorHandleForHeapStart());

	// compute command list
	CMD_LIST->SetComputeRootSignature(COMPUTE_ROOT_SIGNATURE.Get());

	descHeap = gEngine->GetTableDescHeap()->GetUAV().Get();
	CMD_LIST->SetDescriptorHeaps(1, &descHeap);

	D3D12_GPU_DESCRIPTOR_HANDLE handle = descHeap->GetGPUDescriptorHandleForHeapStart();
	CMD_LIST->SetComputeRootDescriptorTable(3, handle);


	mCmdList->ResourceBarrier(1, &barrier);

	mCmdList->RSSetViewports(1, vp);
	mCmdList->RSSetScissorRects(1, rect);
}

void CommandQueue::RenderEnd()
{
	int8 backIndex = mSwapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT);

	mCmdList->ResourceBarrier(1, &barrier);
	mCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { mCmdList.Get() };
	mCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	mSwapChain->Present();

	mSwapChain->SwapIndex();

	CURR_FRAMERESOURCE->mFence = ++mFenceValue;
	mCmdQueue->Signal(mFence.Get(), mFenceValue);
}
