#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
}

Material::~Material()
{
}

void Material::Update()
{
	MATERIAL_CB->CopyData(mMatIndex, mMatData);

	mShader->Update();
}

void Material::PushComputeData()
{
	//MATERIAL_CB->CopyData(mMatIndex, mMatData);

	mShader->Update();
}

void Material::Dispatch(uint32 x, uint32 y, uint32 z)
{
	PushComputeData();

	CMD_LIST->Dispatch(x, y, z);

	//gEngine->GetComputeCmdQueue()->FlushComputeCommandQueue();
}