#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{
}

Shader::~Shader()
{
}

void Shader::LoadGraphicsShader(ShaderInfo info, ShaderPath path)
{
	mInfo = info;

	if (!path.VS.empty())
		mVsBlob = d3dUtil::LoadBinary(path.VS);

	if (!path.PS.empty())
		mPsBlob = d3dUtil::LoadBinary(path.PS);

	if (!path.GS.empty())
		mGsBlob = d3dUtil::LoadBinary(path.GS);

	if (!path.HS.empty())
		mHsBlob = d3dUtil::LoadBinary(path.HS);

	if (!path.DS.empty())
		mDsBlob = d3dUtil::LoadBinary(path.DS);

	CreateGraphicsShader();
}

void Shader::CompileGraphicsShader(ShaderInfo info, const D3D_SHADER_MACRO* define, const wstring& vs, const wstring& ps, const wstring& gs)
{
	mInfo = info;

	if (!vs.empty())
		mVsBlob = d3dUtil::CompileShader(vs, nullptr, "VS_Main", "vs_5_1");

	if (!ps.empty())
		mPsBlob = d3dUtil::CompileShader(ps, define, "PS_Main", "ps_5_1");

	if (!gs.empty())
		mGsBlob = d3dUtil::CompileShader(gs, nullptr, "GS_Main", "gs_5_1");

	CreateGraphicsShader();
}

void Shader::LoadComputeShader(const wstring& cs)
{
	if (!cs.empty())
		mCsBlob = d3dUtil::LoadBinary(cs);

	CreateComputeShader();
}

void Shader::CompileComputeShader(const wstring& cs)
{
	if (!cs.empty())
		mCsBlob = d3dUtil::CompileShader(cs, nullptr, "CS_Main", "cs_5_1");

	CreateComputeShader();
}

void Shader::CreateGraphicsShader()
{
	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	mGraphicsPipelineDesc.InputLayout = { desc, _countof(desc) };
	mGraphicsPipelineDesc.pRootSignature = GRAPHICS_ROOT_SIGNATURE.Get();
	mGraphicsPipelineDesc.VS =
	{
		reinterpret_cast<BYTE*>(mVsBlob->GetBufferPointer()),
		mVsBlob->GetBufferSize()
	};
	mGraphicsPipelineDesc.PS =
	{
		reinterpret_cast<BYTE*>(mPsBlob->GetBufferPointer()),
		mPsBlob->GetBufferSize()
	};
	if (mGsBlob != nullptr) {
		mGraphicsPipelineDesc.GS =
		{
			reinterpret_cast<BYTE*>(mGsBlob->GetBufferPointer()),
			mGsBlob->GetBufferSize()
		};
	}
	if (mDsBlob != nullptr) {
		mGraphicsPipelineDesc.DS =
		{
			reinterpret_cast<BYTE*>(mDsBlob->GetBufferPointer()),
			mDsBlob->GetBufferSize()
		};
	}
	if (mHsBlob != nullptr) {
		mGraphicsPipelineDesc.HS =
		{
			reinterpret_cast<BYTE*>(mHsBlob->GetBufferPointer()),
			mHsBlob->GetBufferSize()
		};
	}
	mGraphicsPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	mGraphicsPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	mGraphicsPipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	mGraphicsPipelineDesc.SampleMask = UINT_MAX;
	mGraphicsPipelineDesc.PrimitiveTopologyType = GetTopologyType(mInfo.TopologyType);
	mGraphicsPipelineDesc.NumRenderTargets = 1;
	mGraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	mGraphicsPipelineDesc.SampleDesc.Count = 1;
	mGraphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (mInfo.ShaderType)
	{
	case SHADER_TYPE::FORWARD:
		mGraphicsPipelineDesc.NumRenderTargets = 1;
		mGraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case SHADER_TYPE::DEFERRED:
		mGraphicsPipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_COUNT;
		mGraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		mGraphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		mGraphicsPipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		mGraphicsPipelineDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		mGraphicsPipelineDesc.RTVFormats[4] = DXGI_FORMAT_R8_UNORM;
		break;
	case SHADER_TYPE::LIGHTING:
		mGraphicsPipelineDesc.NumRenderTargets = 2;
		mGraphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		mGraphicsPipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	default:
		break;
	}


	switch (mInfo.RasterizerType)
	{
	case RASTERIZER_TYPE::CULL_NONE:
		mGraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		mGraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		mGraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		mGraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_BACK:
		mGraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		mGraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		mGraphicsPipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		mGraphicsPipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	default:
		break;
	}

	switch (mInfo.DepthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = FALSE;
		mGraphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	case DEPTH_STENCIL_TYPE::LESS_NO_WRITE:
		mGraphicsPipelineDesc.DepthStencilState.DepthEnable = TRUE;
		mGraphicsPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		mGraphicsPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	default:
		break;
	}

	D3D12_RENDER_TARGET_BLEND_DESC& rt = mGraphicsPipelineDesc.BlendState.RenderTarget[0];

	switch (mInfo.BlendType)
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


	DEVICE->CreateGraphicsPipelineState(&mGraphicsPipelineDesc, IID_PPV_ARGS(&mPipelineState));
}

void Shader::CreateComputeShader()
{
	mInfo.ShaderType = SHADER_TYPE::COMPUTE;

	mComputePipelineDesc.CS =
	{
		reinterpret_cast<BYTE*>(mCsBlob->GetBufferPointer()),
		mCsBlob->GetBufferSize()
	};

	mComputePipelineDesc.pRootSignature = COMPUTE_ROOT_SIGNATURE.Get();

	ThrowIfFailed(DEVICE->CreateComputePipelineState(&mComputePipelineDesc, IID_PPV_ARGS(&mPipelineState)));
}

void Shader::Update()
{
	if (GetShaderType() == SHADER_TYPE::COMPUTE)
		COMPUTE_CMD_LIST->SetPipelineState(mPipelineState.Get());
	else
	{
		GRAPHICS_CMD_LIST->IASetPrimitiveTopology(mInfo.TopologyType);
		GRAPHICS_CMD_LIST->SetPipelineState(mPipelineState.Get());
	}
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE Shader::GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology)
{
	switch (topology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	default:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	}
}