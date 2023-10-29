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
	_lightInfo.position = GetTransform()->GetWorldPosition();
}

void Light::Render()
{
	GetTransform()->PushData();

	_lightMaterial->SetLightIndex(_lightIndex);
	_lightMaterial->Update();

	switch (static_cast<LIGHT_TYPE>(_lightInfo.lightType))
	{
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * _lightInfo.fallOffEnd;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	_volumeMesh->Render();
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>("Rectangle");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>("DirLight");
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>("Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>("PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->Get<Mesh>("Sphere");
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>("SpotLight");
		break;
	}

	GetGameObject()->SetMatIndex(_lightMaterial->GetMatCBIndex());
}
