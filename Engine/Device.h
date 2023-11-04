#pragma once

class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return mDxgi; }
	ComPtr<ID3D12Device> GetDevice() { return mDevice; }

private:
	ComPtr<ID3D12Debug>			mDebugController;
	ComPtr<IDXGIFactory>		mDxgi;
	ComPtr<ID3D12Device>		mDevice; 
};

