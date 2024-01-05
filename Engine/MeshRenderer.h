#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	void Render();
	void RenderShadow();

public:
	virtual void Start() override;

public:
	void SetMesh(sptr<Mesh> mesh) { mMesh = mesh; }
	void SetMaterial(sptr<Material> material, uint32 index = 0);

	sptr<Material> GetMaterial(uint32 index = 0) { return mMaterials[index]; }

	BoundingOrientedBox& GetBoundingBox();


private:
	sptr<Mesh>					mMesh;
	vector<sptr<Material>>		mMaterials;
};

