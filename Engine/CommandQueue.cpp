#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	_cmdList->Close();

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	_fenceValue++;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	auto cmdAlloc = CURR_FRAMERESOURCE->CmdAlloc;

	cmdAlloc->Reset();
	_cmdList->Reset(cmdAlloc.Get(), nullptr);

	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetResource().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	ID3D12DescriptorHeap* descHeap = gEngine->GetTableDescHeap()->GetSRV().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	auto passCB = CURR_FRAMERESOURCE->PassCB->Resource();
	_cmdList->SetGraphicsRootConstantBufferView(0, passCB->GetGPUVirtualAddress());

	auto matData = MATERIAL_CB->Resource();
	_cmdList->SetGraphicsRootShaderResourceView(2, matData->GetGPUVirtualAddress());

	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(DESCHEAP->GetSRV()->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(DESCHEAP->GetSkyTexHeapIndex(), DESCHEAP->GetCbvSrvDescriptorSize());
	_cmdList->SetGraphicsRootDescriptorTable(3, skyTexDescriptor);

	_cmdList->SetGraphicsRootDescriptorTable(4, DESCHEAP->GetSRV()->GetGPUDescriptorHandleForHeapStart());

	_cmdList->ResourceBarrier(1, &barrier);

	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);
}

void CommandQueue::RenderEnd()
{
	int8 backIndex = _swapChain->GetBackBufferIndex();


	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PRESENT);

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	_swapChain->SwapIndex();

	CURR_FRAMERESOURCE->Fence = ++_fenceValue;
	_cmdQueue->Signal(_fence.Get(), _fenceValue);
}
