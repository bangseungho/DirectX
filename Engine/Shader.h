#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	FORWARD,
	DEFERRED,
};

enum class RASTERIGER_TYPE : uint8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(O)
	NO_DEPTH_TEST_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(X)
	LESS_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(O) + ±Ì¿Ã ±‚∑œ(X)
};

enum class BLEND_TYPE : uint8
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIGER_TYPE rasterizerType = RASTERIGER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE dpethStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

public:
	void Init(const wstring& vsPath, const wstring& psPath, ShaderInfo info = ShaderInfo());
	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

private:
	ComPtr<ID3DBlob> CreateShader(const wstring& path);
	ComPtr<ID3DBlob> CreateVertexShader(const wstring& path);
	ComPtr<ID3DBlob> CreatePixelShader(const wstring& path);

private:
	ShaderInfo							_info;

	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

