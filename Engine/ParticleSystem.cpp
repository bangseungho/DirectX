#include "pch.h"
#include "ParticleSystem.h"
#include "Resources.h"
#include "Transform.h"
#include "Mesh.h"

ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	mMesh = GET_SINGLE(Resources)->LoadPointMesh();
	mMaterial = GET_SINGLE(Resources)->Get<Material>("particle");
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

}

void ParticleSystem::Render()
{
	GetTransform()->PushData();

	mMaterial->Update();
	mMesh->Render(mMaxParticle);
}
