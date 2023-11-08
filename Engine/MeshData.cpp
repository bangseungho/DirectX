#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

sptr<MeshData> MeshData::LoadFromFBX(const wstring& path)
{
	FBXLoader loader;
	loader.LoadFbx(path);

	sptr<MeshData> meshData = make_shared<MeshData>();
	
	for (int32 i = 0; i < loader.GetMeshCount(); ++i) {
		sptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i));

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		vector<sptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); ++j) {
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info{};
		info.Mesh = mesh;
		info.Materials = materials;
		meshData->mMeshRenders.push_back(info);
	}

	return meshData;
}

void MeshData::Load(const wstring& path)
{
}

void MeshData::Save(const wstring& path)
{
}

vector<sptr<GameObject>> MeshData::Instantiate()
{
	vector<sptr<GameObject>> v;
	
	for (MeshRenderInfo& info : mMeshRenders) {
		sptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.Mesh);

		for (uint32 i = 0; i < info.Materials.size(); ++i)
			gameObject->GetMeshRenderer()->SetMaterial(info.Materials[i], i);

		v.push_back(gameObject);
	}

	return v;
}
