#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	sptr<Mesh>				Mesh;
	vector<sptr<Material>>	Materials;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	static sptr<MeshData> LoadFromFBX(const wstring& path);
	
	virtual void Load(const wstring& path);
	virtual void Save(const wstring& path);

	vector<sptr<GameObject>> Instantiate();

private:
	sptr<Mesh>				mMesh;
	vector<sptr<Material>>	mMaterials;
	vector<MeshRenderInfo>	mMeshRenders;
};

