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

	ComPtr<ID3DBlob> _vsBlob = CreateVertexShader(vsPath, "vs", "vs_5_1");
	ComPtr<ID3DBlob> _psBlob = CreatePixelShader(psPath, "ps", "ps_5_1");

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	_pipelineDesc.InputLayout = { desc, _countof(desc) };
	_pipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();
	_pipelineDesc.VS =
	{
		reinterpret_cast<BYTE*>(_vsBlob->GetBufferPointer()),
		_vsBlob->GetBufferSize()
	};
	_pipelineDesc.PS =
	{
		reinterpret_cast<BYTE*>(_psBlob->GetBufferPointer()),
		_psBlob->GetBufferSize()
	};
	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = info.topologyType;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (info.shaderType)
	{
	case SHADER_TYPE::FORWARD:
		_pipelineDesc.NumRenderTargets = 1;
		_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case SHADER_TYPE::DEFERRED:
		_pipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_COUNT;
		_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//_pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//_pipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//_pipelineDesc.RTVFormats[3] = DXGI_FORMAT_R8G8B8A8_UNORM;
		//_pipelineDesc.RTVFormats[4] = DXGI_FORMAT_R32_FLOAT;
		break;
	default:
		break;
	}


	switch (info.rasterizerType)
	{
	case RASTERIGER_TYPE::CULL_NONE:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIGER_TYPE::CULL_FRONT:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIGER_TYPE::CULL_BACK:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIGER_TYPE::WIREFRAME:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	default:
		break;
	}

	switch (info.dpethStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	default:
		break;
	}

	DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
}

void Shader::Update()
{
	CMD_LIST->SetPipelineState(_pipelineState.Get());
}

ComPtr<ID3DBlob> Shader::CreateShader(const wstring& path, const string& name, const string& version, D3D12_SHADER_BYTECODE& shaderByteCode)
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

ComPtr<ID3DBlob> Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	return CreateShader(path, name, version, _pipelineDesc.VS);
}

ComPtr<ID3DBlob> Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	return CreateShader(path, name, version, _pipelineDesc.PS);
}