#pragma once
#include "Component.h"
#include "UploadBuffer.h"
#include "StructuredBuffer.h"

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	void SetParticleIndex(int32 particleIndex) { mParticleIndex = particleIndex; }
	void SetMaterial(sptr<class Material> material) { mMaterial = material; }
	void SetComputeMaterial(sptr<class Material> material) { mComputeMaterial = material; }
	ParticleSystemData& GetParticleSystemData() { return mParticleSystemData; }

	virtual void Start();
	virtual void FinalUpdate();
	void Render();
	
protected:
	sptr<class Material>	mMaterial;
	sptr<class Material>	mComputeMaterial;
	sptr<class Mesh>		mMesh;

	float mCreateInterval = 0.005f;
	ParticleSystemData 	mParticleSystemData = {
		500, 0, 0.f, 500, 0.5f, 5.f, 100, 50, 10.f, 1.f
	};

	sptr<StructuredBuffer> mParticleData;
	sptr<StructuredBuffer> mParticleSharedData;

	int32 mParticleIndex = -1;

	sptr<UploadBuffer<ParticleSharedData>> mSharedDataBuffer;
};

class HitParticleSystem : public ParticleSystem
{
public:
	HitParticleSystem();
	virtual ~HitParticleSystem();

	virtual void FinalUpdate();

private:
	float mLifeTime = 0.f;
};