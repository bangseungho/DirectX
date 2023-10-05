#pragma once

class Mesh
{
public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);
	void Render();

	void SetObjectConstant(const ObjectConstants& o) { _objectConstant = o; }
	void SetTexture(shared_ptr<class Texture> tex) { _tex = tex; }

public:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

private:
	ComPtr<ID3D12Resource> _vertexBuffer;
	ComPtr<ID3D12Resource> _vertexBufferUploader;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};
	uint32 _vertexCount = 0;

	ComPtr<ID3D12Resource> _indexBuffer;
	ComPtr<ID3D12Resource> _indexBufferUploader;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView = {};
	uint32 _indexCount = 0;

	ObjectConstants _objectConstant = {};
	shared_ptr<class Texture> _tex = {};
};

