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
	mComputeMaterial = GET_SINGLE(Resources)->Get<Material>("Compute_Spread_Particle");

	mSharedDataBuffer = std::make_shared<UploadBuffer<ParticleSharedData>>(DEVICE, 1, false);

	mParticleData = std::make_shared<StructuredBuffer>();
	mParticleData->Init(sizeof(ParticleData), mParticleSystemData.MaxCount);

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
	mParticleSystemData.AccTime += DELTA_TIME;

	int32 addCount = 0;
	if (mCreateInterval < mParticleSystemData.AccTime) {
		mParticleSystemData.AccTime = mParticleSystemData.AccTime - mCreateInterval;
		addCount = 1;
	}

	mParticleSystemData.AddCount = addCount;
	mParticleSystemData.DeltaTime = DELTA_TIME;
	PARTICLE_SYSTEM_DATA->CopyData(mParticleIndex, mParticleSystemData);

	mParticleData->PushComputeData(UAV_REGISTER::u1);
	mParticleSharedData->PushComputeData(UAV_REGISTER::u2);

	mComputeMaterial->Dispatch(1, 1, 1);
}

void ParticleSystem::Render()
{
	GetTransform()->PushData();

	mMaterial->Update();
	mParticleData->PushGraphicsData(5);
	mMesh->Render(mParticleSystemData.MaxCount);
}
