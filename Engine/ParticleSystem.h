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
	virtual void Start();
	virtual void FinalUpdate();
	void Render();
	
private:
	shared_ptr<class Material>	mMaterial;
	shared_ptr<class Material>	mComputeMaterial;
	shared_ptr<class Mesh>		mMesh;

	uint32	mMaxParticle = 1000;
	float	mCreateInterval = 0.005f;
	float	mAccTime = 0.f;
	float	mMinLifeTime = 0.5f;
	float	mMaxLifeTime = 1.f;
	float	mMinSpeed = 100;
	float	mMaxSpeed = 50;
	float	mStartScale = 10.f;
	float	mEndScale = 1.f;

	sptr<StructuredBuffer> mParticleData;
	sptr<StructuredBuffer> mParticleSharedData;

	sptr<UploadBuffer<ParticleSharedData>> mSharedDataBuffer;
};

