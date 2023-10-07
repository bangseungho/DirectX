#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void SetMesh(sptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(sptr<Material> material) { _material = material; }

	virtual void Update() override { Render(); }

	void Render();

private:
	sptr<Mesh> _mesh;
	sptr<Material> _material;
};

