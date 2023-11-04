#pragma once
#include "Object.h"

class Material : public Object
{
public:
	Material();
	virtual ~Material();

public:
	void SetShader(shared_ptr<class Shader> shader) { mShader = shader; }
	shared_ptr<class Shader> GetShader() { return mShader; }

	void SetDiffuse(Vec4 diffuse) { mMatData.DiffuseAlbedo = diffuse; }
	void SetFresnel(Vec3 fresnel) { mMatData.FresnelR0 = fresnel; }
	void SetRoughness(float roughness) { mMatData.Roughness = roughness; }
	void SetMatTransform(Matrix matTransform) { mMatData.MatTransform = matTransform; }
	void SetLightIndex(uint8 index) { mMatData.LightIndex = index; }

	void SetMatCBIndex(uint32 index) { mMatIndex = index; }
	uint32 GetMatCBIndex() const { return mMatIndex; }

	void SetDiffuseSrvHeapIndex(uint8 index) { mMatData.TextureMapIndex = index; }
	void SetNormalSrvHeapIndex(uint8 index) { mMatData.NormalMapIndex = static_cast<uint8>(index); }
	void SetRoughnessSrvHeapIndex(uint8 index) { mMatData.RoughnessMapIndex = static_cast<uint8>(index); }

	void Dispatch(uint32 x, uint32 y, uint32 z);

	void Update();
	void PushComputeData();

private:
	shared_ptr<class Shader>	mShader;
	MaterialData				mMatData;
	uint32						mMatIndex = 0;
};

