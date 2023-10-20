#pragma once
#include "Object.h"

enum
{
	MATERIAL_PARAM_COUNT = 5,
	MATERIAL_TEXTURE_COUNT = 5,
};

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
	void SetNormalMapping(float normalMapping) { _params.NormalMapping = normalMapping; }
	void SetMatTransform(Matrix matTransform) { _params.MatTransform = matTransform; }
	void SetTexture(uint8 index, shared_ptr<class Texture> texture) { _textures[index] = texture; }

	void SetMatCBIndex(uint32 index) { _matCBIndex = index; }
	uint32 GetMatCBIndex() const { return _matCBIndex; }

	void SetDiffuseSrvHeapIndex(TEXTURE2D_INDEX index) { _params.DiffuseMapIndex = static_cast<uint8>(index); }
	void SetNormalSrvHeapIndex(TEXTURE2D_INDEX index) { _params.NormalMapIndex = static_cast<uint8>(index); }
	void SetRoughnessSrvHeapIndex(TEXTURE2D_INDEX index) { _params.RoughnessMapIndex = static_cast<uint8>(index); }
	
	void SetDiffuseSrvHeapIndex(TEXTURECUBE_INDEX index) { _params.DiffuseMapIndex = static_cast<uint8>(index); }

	void Update();

private:
	shared_ptr<class Shader> _shader;
	MaterialConstants _params;
	array<shared_ptr<class Texture>, MATERIAL_TEXTURE_COUNT> _textures;

	uint32 _matCBIndex = 0;
};

