#pragma once

class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

	array<const CD3DX12_STATIC_SAMPLER_DESC, STATIC_SAMPLER_COUNT> GetStaticSamplers();

private:
	ComPtr<ID3D12RootSignature>	_signature;
};

