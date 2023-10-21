#pragma once
#include "Object.h"

enum class RASTERIGER_TYPE
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIGER_TYPE rasterizerType = RASTERIGER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE dpethStencilType = DEPTH_STENCIL_TYPE::LESS;
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

public:
	void Init(const wstring& vsPath, const wstring& psPath, ShaderInfo info = ShaderInfo());
	void Update();

private:
	ComPtr<ID3DBlob> CreateShader(const wstring& path, const string& name, const string& version, D3D12_SHADER_BYTECODE& shaderByteCode);
	ComPtr<ID3DBlob> CreateVertexShader(const wstring& path, const string& name, const string& version);
	ComPtr<ID3DBlob> CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

