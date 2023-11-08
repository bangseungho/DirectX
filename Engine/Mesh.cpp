#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Texture.h"
#include "Material.h"
#include "FBXLoader.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{
}

Mesh::~Mesh()
{
}

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render(uint32 instanceCount, uint32 idx)
{
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &mVertexBufferView);
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&mIndexBufferInfos[idx].BufferView);

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(mIndexBufferInfos[idx].Count, instanceCount, 0, 0, 0);
}

sptr<Mesh> Mesh::CreateFromFBX(const FbxMeshInfo* meshInfo)
{
	sptr<Mesh> mesh = make_shared<Mesh>();

	mesh->CreateVertexBuffer(meshInfo->vertices);

	for (const vector<uint32>& buffer : meshInfo->indices) {
		if (buffer.empty()) {
			vector<uint32> defaultBuffer{ 0 };
			mesh->CreateIndexBuffer(defaultBuffer);
		}
		else {
			mesh->CreateIndexBuffer(buffer);
		}
	}

	return mesh;
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	mVertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = mVertexCount * sizeof(Vertex);

	mVertexBuffer = d3dUtil::CreateDefaultBuffer(DEVICE, GRAPHICS_CMD_LIST, buffer.data(), bufferSize, mVertexBufferUploader);

	mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = sizeof(Vertex);
	mVertexBufferView.SizeInBytes = bufferSize;
}


void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	uint32 indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = indexCount * sizeof(uint32);

	ComPtr<ID3D12Resource> indexBufferUploader;
	ComPtr<ID3D12Resource> indexBuffer = d3dUtil::CreateDefaultBuffer(DEVICE, GRAPHICS_CMD_LIST, buffer.data(), bufferSize, indexBufferUploader);

	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = bufferSize;

	IndexBufferInfo info =
	{
		indexBuffer,
		indexBufferUploader,
		indexBufferView,
		DXGI_FORMAT_R32_UINT,
		indexCount
	};

	mIndexBufferInfos.push_back(info);
}
