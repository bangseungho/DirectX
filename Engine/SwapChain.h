#pragma once

class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

public:
	ComPtr<IDXGISwapChain> GetSwapChain() { return mSwapChain; }
	uint8 GetBackBufferIndex() { return mBackBufferIndex; }

private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

private:
	ComPtr<IDXGISwapChain>	mSwapChain;
	uint32					mBackBufferIndex = 0;
};

