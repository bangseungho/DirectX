#pragma once

class Mesh
{
public:
	void Init(vector<Vertex>& vec);
	void Render();

	void SetTransform(const Transform& t) { _transform = t; }
	void SetObjCBIndex(UINT i) { _objCBIndex = i; }

private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	Transform _transform = {};

	UINT _objCBIndex = -1;
};

