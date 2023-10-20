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

void MeshRenderer::SetMaterial(sptr<Material> material)
{
	_material = material;
}

BoundingOrientedBox& MeshRenderer::GetBoundingBox()
{
	return _mesh->GetBoundingBox();
}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	_material->Update();
	_mesh->Render();
}

void MeshRenderer::Start()
{
	GetGameObject()->SetMatIndex(_material->GetMatCBIndex());
}
