#include "pch.h"
#include "ParticleSystem.h"
#include "Resources.h"
#include "Transform.h"
#include "Mesh.h"
#include "Timer.h"
#include "Engine.h"

ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	mMesh = GET_SINGLE(Resources)->LoadPointMesh();
	mMaterial = GET_SINGLE(Resources)->Get<Material>("lightParticle");

	mComputeMaterial = GET_SINGLE(Resources)->Get<Material>("ComputeParticle");

	mSharedDataBuffer = std::make_shared<UploadBuffer<ParticleSharedData>>(DEVICE, 1, false);

	mParticleData = std::make_shared<StructuredBuffer>();
	mParticleData->Init(sizeof(ParticleData), mMaxParticle);

	mParticleSharedData = std::make_shared<StructuredBuffer>();
	mParticleSharedData->Init(sizeof(ParticleSharedData), 1);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Start()
{
	GetGameObject()->SetMatIndex(mMaterial->GetMatCBIndex());
}

void ParticleSystem::FinalUpdate()
{
	mAccTime += DELTA_TIME;

	int32 addCount = 0;
	if (mCreateInterval < mAccTime) {
		mAccTime = mAccTime - mCreateInterval;
		addCount = 1;
	}

	ParticleSystemData particleSystemData;
	particleSystemData.MaxCount = mMaxParticle;
	particleSystemData.AddCount = addCount;
	particleSystemData.AccTime = mAccTime;
	particleSystemData.DeltaTime = DELTA_TIME;
	particleSystemData.MinLifeTime = mMinLifeTime;
	particleSystemData.MaxLifeTime = mMaxLifeTime;
	particleSystemData.MinSpeed = mMinSpeed;
	particleSystemData.MaxSpeed = mMaxSpeed;
	particleSystemData.StartScale = mStartScale;
	particleSystemData.EndScale = mEndScale;
	PARTICLE_SYSTEM_DATA->CopyData(0, particleSystemData);

	mParticleData->PushComputeData(UAV_REGISTER::u1);
	mParticleSharedData->PushComputeData(UAV_REGISTER::u2);

	mComputeMaterial->Dispatch(1, 1, 1);
}

void ParticleSystem::Render()
{
	GetTransform()->PushData();

	mMaterial->Update();
	mParticleData->PushGraphicsData(5);
	mMesh->Render(mMaxParticle);
}
