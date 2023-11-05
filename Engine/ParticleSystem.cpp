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
	mMaterial = GET_SINGLE(Resources)->Get<Material>("Particle");
	mComputeMaterial = GET_SINGLE(Resources)->Get<Material>("ComputeParticle");

	mSharedDataBuffer = std::make_shared<UploadBuffer<ParticleSharedData>>(DEVICE, 1, false);
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

	ParticleSharedData particleSaredData;
	particleSaredData.AddCount = 1;
	mSharedDataBuffer->CopyData(0, particleSaredData);
	COMPUTE_CMD_LIST->SetComputeRootUnorderedAccessView(1, mSharedDataBuffer->Resource()->GetGPUVirtualAddress());

	ParticleSystemData particleSystemData;
	particleSystemData.MaxCount = TOTAL_TIME;
	particleSystemData.AccTime = mAccTime;
	particleSystemData.DeltaTime = DELTA_TIME;
	particleSystemData.MinLifeTime = mMinLifeTime;
	particleSystemData.MaxLifeTime = mMaxLifeTime;
	particleSystemData.MinSpeed = mMinSpeed;
	particleSystemData.MaxSpeed = mMaxSpeed;

	PARTICLE_SYSTEM_DATA->CopyData(0, particleSystemData);
}

void ParticleSystem::Render()
{
	mComputeMaterial->Dispatch(1, 1024, 1);
	GetTransform()->PushData();

	mMaterial->Update();
	mMesh->Render(mMaxParticle);
}
