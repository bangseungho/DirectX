#pragma once
#include "Object.h"

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		Buffer;
	ComPtr<ID3D12Resource>		BufferUploader;
	D3D12_INDEX_BUFFER_VIEW		BufferView;
	DXGI_FORMAT					Format;
	uint32						Count;
};


class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

public:
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer);
	void Render(uint32 instanceCount = 1, uint32 idx = 0);

public:
	static sptr<Mesh> CreateFromFBX(const struct FbxMeshInfo* meshInfo);

	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

public:
	BoundingOrientedBox& GetBoundingBox() { return mBoundingBox; }
	uint32 GetSubSetCount() { return static_cast<uint32>(mIndexBufferInfos.size()); }

private:
	ComPtr<ID3D12Resource>			mVertexBuffer;
	ComPtr<ID3D12Resource>			mVertexBufferUploader;
	D3D12_VERTEX_BUFFER_VIEW		mVertexBufferView = {};
	uint32							mVertexCount = 0;

	vector<IndexBufferInfo>			mIndexBufferInfos;

	ObjectData						mObjectData = {};
	BoundingOrientedBox				mBoundingBox;
};

