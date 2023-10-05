#pragma once

class Mesh
{
public:
	void Init(vector<Vertex>& vec);
	void Render();

	void SetTransform(const ObjectConstants& t) { _transform = t; }

private:
	ComPtr<ID3D12Resource> _vertexBuffer			= nullptr;
	ComPtr<ID3D12Resource> _vertexBufferUploader	= nullptr;

	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};

	uint32 _vertexCount = 0;

	ObjectConstants _transform = {};

	UINT _objCBIndex = -1;
};

