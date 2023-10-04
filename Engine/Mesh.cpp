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
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::Render()
{
	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	// TODO
	// 1) Buffer에다가 데이터 세팅
	// 2) Buffer의 주소를 register에다가 전송
	ObjectConstants objConstants;
	objConstants.offset = _transform.offset;

	CURR_FRAME_RESOURCE->ObjectCB->CopyData(_objCBIndex, objConstants);

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = OBJECT_CB->GetGPUVirtualAddress() + _objCBIndex * objCBByteSize;

	CMD_LIST->SetGraphicsRootConstantBufferView(0, objCBAddress);

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}