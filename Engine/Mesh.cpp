#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Texture.h"
#include "Material.h"

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

void Mesh::Render(uint32 instanceCount)
{
	CMD_LIST->IASetVertexBuffers(0, 1, &mVertexBufferView);
	CMD_LIST->IASetIndexBuffer(&mIndexBufferView);

	CMD_LIST->DrawIndexedInstanced(mIndexCount, instanceCount, 0, 0, 0);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	mVertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = mVertexCount * sizeof(Vertex);

	mVertexBuffer = d3dUtil::CreateDefaultBuffer(DEVICE, CMD_LIST, buffer.data(), bufferSize, mVertexBufferUploader);

	mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = sizeof(Vertex);
	mVertexBufferView.SizeInBytes = bufferSize;
}


void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	mIndexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = mIndexCount * sizeof(uint32);

	mIndexBuffer = d3dUtil::CreateDefaultBuffer(DEVICE, CMD_LIST, buffer.data(), bufferSize, mIndexBufferUploader);

	mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	mIndexBufferView.SizeInBytes = bufferSize;
}
