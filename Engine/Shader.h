#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	FORWARD,
	DEFERRED,
	LIGHTING,
	COMPUTE,
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
	NO_DEPTH_TEST, // ���� �׽�Ʈ(X) + ���� ���(O)
	NO_DEPTH_TEST_NO_WRITE, // ���� �׽�Ʈ(X) + ���� ���(X)
	LESS_NO_WRITE, // ���� �׽�Ʈ(O) + ���� ���(X)
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
	void LoadGraphicsShader(ShaderInfo info, const wstring& vs = L"", const wstring& ps = L"", const wstring& gs = L"");
	void CompileGraphicsShader(ShaderInfo info, const D3D_SHADER_MACRO* define = nullptr, const wstring& vs = L"", const wstring& ps = L"", const wstring& gs = L"");
	void LoadComputeShader(const wstring& cs = L"");
	void CompileComputeShader(const wstring& cs = L"");

	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

private:
	void CreateComputeShader();
	void CreateGraphicsShader();

private:
	ShaderInfo							_info;
	ComPtr<ID3D12PipelineState>			_pipelineState;

	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_errBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _graphicsPipelineDesc = {};

private:
	ComPtr<ID3DBlob>					_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC  _computePipelineDesc = {};
};

