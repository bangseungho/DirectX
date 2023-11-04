#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void SetMesh(sptr<Mesh> mesh) { mMesh = mesh; }
	void SetMaterial(sptr<Material> material) { mMaterial = material; }
	sptr<Material> GetMaterial() { return mMaterial; }
	BoundingOrientedBox& GetBoundingBox();

	void Render();

public:
	virtual void Start() override;

private:
	sptr<Mesh>			mMesh;
	sptr<Material>		mMaterial;
};

