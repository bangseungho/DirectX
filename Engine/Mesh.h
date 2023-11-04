#pragma once
#include "Object.h"

class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);
	void Render();

	BoundingOrientedBox& GetBoundingBox() { return mBoundingBox; }

public:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

private:
	ComPtr<ID3D12Resource>			mVertexBuffer;
	ComPtr<ID3D12Resource>			mVertexBufferUploader;
	D3D12_VERTEX_BUFFER_VIEW		mVertexBufferView = {};
	uint32							mVertexCount = 0;

	ComPtr<ID3D12Resource>			mIndexBuffer;
	ComPtr<ID3D12Resource>			mIndexBufferUploader;
	D3D12_INDEX_BUFFER_VIEW			mIndexBufferView = {};
	uint32							mIndexCount = 0;

	ObjectData						mObjectData = {};

	BoundingOrientedBox				mBoundingBox;
};

