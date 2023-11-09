#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Resources.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	mLightInfo.Position = GetTransform()->GetWorldPosition();
}

void Light::Render()
{
	GetTransform()->PushData();

	mLightMaterial->Update();

	switch (static_cast<LIGHT_TYPE>(mLightInfo.LightType))
	{
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * mLightInfo.FallOffEnd;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	mVolumeMesh->Render();
}

void Light::SetLightType(LIGHT_TYPE type)
{
	mLightInfo.LightType = static_cast<int32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		mVolumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle");
		mLightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		mVolumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		mLightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		mVolumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Sphere");
		mLightMaterial = GET_SINGLE(Resources)->Get<Material>(L"SpotLight");
		break;
	}

	GetGameObject()->SetMatIndex(mLightMaterial->GetMatCBIndex());
}
