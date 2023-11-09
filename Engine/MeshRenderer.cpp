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

void MeshRenderer::Render()
{
	for (uint32 i = 0; i < mMaterials.size(); ++i) {
		sptr<Material>& material = mMaterials[i];

		if (material == nullptr || material->GetShader() == nullptr)
			continue;

		if (!GetGameObject()->GetActive())
			continue;

		GetTransform()->PushData();
		material->Update();
		mMesh->Render(1, i);
	}
}

void MeshRenderer::Start()
{
	for (uint32 i = 0; i < mMaterials.size(); ++i) {
		sptr<Material>& material = mMaterials[i];

		if (material == nullptr || material->GetShader() == nullptr)
			continue;

		GetGameObject()->SetMatIndex(material->GetMatCBIndex());
	}
}

void MeshRenderer::SetMaterial(sptr<Material> material, uint32 index)
{
	if (mMaterials.size() <= static_cast<size_t>(index))
		mMaterials.resize(static_cast<size_t>(index + 1));

	mMaterials[index] = material;
}

BoundingOrientedBox& MeshRenderer::GetBoundingBox()
{
	return mMesh->GetBoundingBox();
}