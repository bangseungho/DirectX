#pragma once
#include "Object.h"

class Material : public Object
{
public:
	Material();
	virtual ~Material();

public:
	void SetShader(shared_ptr<class Shader> shader) { _shader = shader; }
	shared_ptr<class Shader> GetShader() { return _shader; }

	void SetDiffuse(Vec4 diffuse) { _params.DiffuseAlbedo = diffuse; }
	void SetFresnel(Vec3 fresnel) { _params.FresnelR0 = fresnel; }
	void SetRoughness(float roughness) { _params.Roughness = roughness; }
	void SetMatTransform(Matrix matTransform) { _params.MatTransform = matTransform; }
	void SetLightIndex(uint8 index) { _params.LightIndex = index; }

	void SetMatCBIndex(uint32 index) { _matCBIndex = index; }
	uint32 GetMatCBIndex() const { return _matCBIndex; }

	void SetDiffuseSrvHeapIndex(uint8 index) { _params.TextureMapIndex = index; }
	void SetNormalSrvHeapIndex(uint8 index) { _params.NormalMapIndex = static_cast<uint8>(index); }
	void SetRoughnessSrvHeapIndex(uint8 index) { _params.RoughnessMapIndex = static_cast<uint8>(index); }

	void Update();

private:
	shared_ptr<class Shader> _shader;
	MaterialConstants _params;

	uint32 _matCBIndex = 0;
};

