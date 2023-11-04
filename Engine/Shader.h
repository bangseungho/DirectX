#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	FORWARD,
	DEFERRED,
	LIGHTING,
	COMPUTE,
};

enum class RASTERIZER_TYPE : uint8
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
	SHADER_TYPE						ShaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE					RasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE				DepthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE						BlendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY			TopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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

	SHADER_TYPE GetShaderType() { return mInfo.ShaderType; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	void CreateComputeShader();
	void CreateGraphicsShader();

private:
	ShaderInfo							mInfo;
	ComPtr<ID3D12PipelineState>			mPipelineState;

	ComPtr<ID3DBlob>					mVsBlob;
	ComPtr<ID3DBlob>					mPsBlob;
	ComPtr<ID3DBlob>					mGsBlob;
	ComPtr<ID3DBlob>					mErrBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  mGraphicsPipelineDesc = {};

private:
	ComPtr<ID3DBlob>					mCsBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC	mComputePipelineDesc = {};
};

