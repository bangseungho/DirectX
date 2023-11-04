#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "GameObject.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{

}

MeshRenderer::~MeshRenderer()
{

}

BoundingOrientedBox& MeshRenderer::GetBoundingBox()
{
	return mMesh->GetBoundingBox();
}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	mMaterial->Update();
	mMesh->Render();
}

void MeshRenderer::Start()
{
	GetGameObject()->SetMatIndex(mMaterial->GetMatCBIndex());
}
