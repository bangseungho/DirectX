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
	void SetMaterial(sptr<class Material> material) { mMaterial = material; }

	virtual void Start();
	virtual void FinalUpdate();
	void Render();
	
private:
	sptr<class Material>	mMaterial;
	sptr<class Material>	mComputeMaterial;
	sptr<class Mesh>		mMesh;

	uint32	mMaxParticle = 500;
	float	mCreateInterval = 0.005f;
	float	mAccTime = 0.f;
	float	mMinLifeTime = 0.5f;
	float	mMaxLifeTime = 5.f;
	float	mMinSpeed = 100;
	float	mMaxSpeed = 50;
	float	mStartScale = 10.f;
	float	mEndScale = 1.f;

	sptr<StructuredBuffer> mParticleData;
	sptr<StructuredBuffer> mParticleSharedData;

	sptr<UploadBuffer<ParticleSharedData>> mSharedDataBuffer;
};

