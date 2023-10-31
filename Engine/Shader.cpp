#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{
}

Shader::~Shader()
{
}

void Shader::Init(const wstring& vsPath, const wstring& psPath,  ShaderInfo info)
{
	_info = info;

	_vsBlob = CreateVertexShader(vsPath);
	_psBlob = CreatePixelShader(psPath);

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
	_graphicsPipelineDesc.PrimitiveTopologyType = info.topologyType;
	_graphicsPipelineDesc.NumRenderTargets = 1;
	_graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_graphicsPipelineDesc.SampleDesc.Count = 1;
	_graphicsPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (info.shaderType)
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


	switch (info.rasterizerType)
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

	switch (info.dpethStencilType)
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

	switch (info.blendType)
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

void Shader::Init(const wstring& csPath)
{
	_info.shaderType = SHADER_TYPE::COMPUTE;

	_csBlob = CreateShader(csPath);
	
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

ComPtr<ID3DBlob> Shader::CreateShader(const wstring& path)
{
	ifstream in{ path, std::ios::binary | std::ios::ate };

	in.seekg(0, ios_base::end);
	ifstream::pos_type size = (int)in.tellg();
	in.seekg(0, ios_base::beg);

	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));
	
	in.read((char*)blob->GetBufferPointer(), size);
	in.close();

	return blob;
}

ComPtr<ID3DBlob> Shader::CreateVertexShader(const wstring& path)
{
	return CreateShader(path);
}

ComPtr<ID3DBlob> Shader::CreatePixelShader(const wstring& path)
{
	return CreateShader(path);
}