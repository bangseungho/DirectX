#pragma once

class RootSignature
{
public:
	void Init();

private:
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();

public:
	ComPtr<ID3D12RootSignature>	GetGraphicsRootSignature() { return mGraphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeRootSignature() { return mComputeRootSignature; }
	array<const CD3DX12_STATIC_SAMPLER_DESC, STATIC_SAMPLER_COUNT> GetStaticSamplers();

private:
	ComPtr<ID3D12RootSignature>	mGraphicsRootSignature;
	ComPtr<ID3D12RootSignature>	mComputeRootSignature;
};

