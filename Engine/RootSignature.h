#pragma once

class RootSignature
{
public:
	void Init();

private:
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();

public:
	ComPtr<ID3D12RootSignature>	GetGraphicsRootSignature() { return _graphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeRootSignature() { return _computeRootSignature; }
	array<const CD3DX12_STATIC_SAMPLER_DESC, STATIC_SAMPLER_COUNT> GetStaticSamplers();

private:
	ComPtr<ID3D12RootSignature>	_graphicsRootSignature;
	ComPtr<ID3D12RootSignature>	_computeRootSignature;
};

