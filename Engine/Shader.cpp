#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{
}

Shader::~Shader()
{
}

void Shader::LoadGraphicsShader(ShaderInfo info, const wstring& vs, const wstring& ps, const wstring& gs)
{
	_info = info;

	if (!vs.empty())
		_vsBlob = d3dUtil::LoadBinary(vs);

	if (!ps.empty())
		_psBlob = d3dUtil::LoadBinary(ps);

	if (!gs.empty())
		_gsBlob = d3dUtil::LoadBinary(gs);

	CreateGraphicsShader();
}

void Shader::CompileGraphicsShader(ShaderInfo info, const D3D_SHADER_MACRO* define, const wstring& vs, const wstring& ps, const wstring& gs)
{
	_info = info;

	if (!vs.empty())
		_vsBlob = d3dUtil::CompileShader(vs, nullptr, "VS_Main", "vs_5_1");

	if (!ps.empty())
		_psBlob = d3dUtil::CompileShader(ps, define, "PS_Main", "ps_5_1");

	if (!gs.empty())
		_gsBlob = d3dUtil::CompileShader(gs, nullptr, "GS_Main", "gs_5_1");

	CreateGraphicsShader();
}

void Shader::LoadComputeShader(const wstring& cs)
{
	if (!cs.empty())
		_csBlob = d3dUtil::LoadBinary(cs);

	CreateComputeShader();
}

void Shader::CompileComputeShader(const wstring& cs)
{
	if (!cs.empty())
		_csBlob = d3dUtil::CompileShader(cs, nullptr, "CS_Main", "cs_5_1");

	CreateComputeShader();
}

void Shader::CreateGraphicsShader()
{
	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	_graphicsPipelineDesc.InputLayout = { desc, _countof(desc) };
	_graphicsPipelineDesc.pRootSignature = GRAPHICS_ROOT_SIGNATURE.Get();
	_graphicsPipelineDesc.VS =
	{
		reinterpret_cast<BYTE*>(_vsBlob->GetBufferPointer()),
		_vsBlob->GetBufferSize()
	};
	_graphicsPipelineDesc.PS =
	{
		reinterpret_cast<BYTE*>(_psBlob->GetBufferPointer()),
		_psBlob->GetBufferSize()
	};
	_graphicsPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_graphicsPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_graphicsPipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_graphicsPipelineDesc.SampleMask = UINT_MAX;
	_graphicsPipelineDesc.PrimitiveTopologyType = _info.topologyType;
	_graphicsPipelineDesc.NumRenderTargets = 1;
	_graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_graphicsPipelineDesc.SampleDesc.Count = 1;
	_graphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (_info.shaderType)
	{
	case SHADER_TYPE::FORWARD:
		_graphicsPipelineDesc.NumRenderTargets = 1;
		_graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case SHADER_TYPE::DEFERRED:
		_graphicsPipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_COUNT;
		_graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_graphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_graphicsPipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipelineDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipelineDesc.RTVFormats[4] = DXGI_FORMAT_R8_UNORM;
		break;
	case SHADER_TYPE::LIGHTING:
		_graphicsPipelineDesc.NumRenderTargets = 2;
		_graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		_graphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	default:
		break;
	}


	switch (_info.rasterizerType)
	{
	case RASTERIGER_TYPE::CULL_NONE:
		_graphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_graphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIGER_TYPE::CULL_FRONT:
		_graphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_graphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIGER_TYPE::CULL_BACK:
		_graphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_graphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIGER_TYPE::WIREFRAME:
		_graphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		_graphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	default:
		break;
	}

	switch (_info.dpethStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
		_graphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	case DEPTH_STENCIL_TYPE::LESS_NO_WRITE:
		_graphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_graphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		_graphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	default:
		break;
	}

	D3D12_RENDER_TARGET_BLEND_DESC& rt = _graphicsPipelineDesc.BlendState.RenderTarget[0];

	switch (_info.blendType)
	{
	case BLEND_TYPE::DEFAULT:
		rt.BlendEnable = FALSE;
		rt.LogicOpEnable = FALSE;
		rt.SrcBlend = D3D12_BLEND_ONE;
		rt.DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLEND_TYPE::ALPHA_BLEND:
		rt.BlendEnable = TRUE;
		rt.LogicOpEnable = FALSE;
		rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rt.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		rt.BlendOp = D3D12_BLEND_OP_ADD;
		break;
	case BLEND_TYPE::ONE_TO_ONE_BLEND:
		rt.BlendEnable = TRUE;
		rt.LogicOpEnable = FALSE;
		rt.SrcBlend = D3D12_BLEND_ONE;
		rt.DestBlend = D3D12_BLEND_ONE;
		break;
	}


	DEVICE->CreateGraphicsPipelineState(&_graphicsPipelineDesc, IID_PPV_ARGS(&_pipelineState));
}

void Shader::CreateComputeShader()
{
	_info.shaderType = SHADER_TYPE::COMPUTE;

	_computePipelineDesc.CS =
	{
		reinterpret_cast<BYTE*>(_csBlob->GetBufferPointer()),
		_csBlob->GetBufferSize()
	};

	_computePipelineDesc.pRootSignature = COMPUTE_ROOT_SIGNATURE.Get();

	ThrowIfFailed(DEVICE->CreateComputePipelineState(&_computePipelineDesc, IID_PPV_ARGS(&_pipelineState)));
}

void Shader::Update()
{
	switch (_info.shaderType)
	{
	case SHADER_TYPE::FORWARD:
	case SHADER_TYPE::DEFERRED:
	case SHADER_TYPE::LIGHTING:
		GRAPHICS_CMD_LIST->SetPipelineState(_pipelineState.Get());
		break;
	case SHADER_TYPE::COMPUTE:
		COMPUTE_CMD_LIST->SetPipelineState(_pipelineState.Get());
		break;
	default:
		break;
	}
}


