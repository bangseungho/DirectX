#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	_vertexBuffer = d3dUtil::CreateDefaultBuffer(DEVICE, CMD_LIST, &vec[0], bufferSize, _vertexBufferUploader);

	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); 
	_vertexBufferView.SizeInBytes = bufferSize; 
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	ObjectConstants objConstants;
	objConstants.offset = _transform.offset;
	objConstants.color = _transform.color;
	
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = CURR_OBJECT_CB->PushData(0, &objConstants, objCBByteSize);
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
	}

	GEngine->GetTableDescHeap()->CommitTable();

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}