#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Resources.h"
#include "Camera.h"
#include "SceneManager.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	mShadowCamera = make_shared<GameObject>();
	mShadowCamera->AddComponent(make_shared<Transform>());
	mShadowCamera->AddComponent(make_shared<Camera>());
	uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
	mShadowCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	mLightInfo.Position = GetTransform()->GetWorldPosition();

	mShadowCamera->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	mShadowCamera->GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation());
	mShadowCamera->GetTransform()->SetLocalScale(GetTransform()->GetLocalScale());
	mShadowCamera->FinalUpdate();

	if (static_cast<LIGHT_TYPE>(mLightInfo.LightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT)
		Camera::sMatShadowViewProj = mShadowCamera->GetCamera()->GetViewMatrix() * mShadowCamera->GetCamera()->GetProjectionMatrix();
}

void Light::Render()
{
	GetTransform()->PushData();

	if (static_cast<LIGHT_TYPE>(mLightInfo.LightType) == LIGHT_TYPE::DIRECTIONAL_LIGHT) {
		mLightMaterial->SetShadowMapHeapIndex(GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget")->GetTexHeapIndex());

		Matrix MatVP = mShadowCamera->GetCamera()->GetViewMatrix() * mShadowCamera->GetCamera()->GetProjectionMatrix();
		mLightMaterial->SetMatShadowCameraViewProj(MatVP);
	}
	else {
		float scale = 2 * mLightInfo.FallOffEnd;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
	}

	mLightMaterial->Update();

	mVolumeMesh->Render();
}

void Light::RenderShadow()
{
	mShadowCamera->GetCamera()->SortShadowObject();
	mShadowCamera->GetCamera()->Render_Shadow();
}

void Light::SetLightDirection(Vec3 direction)
{
	direction.Normalize();

	mLightInfo.Direction = direction;

	GetTransform()->LookAt(direction);
}

void Light::SetLightType(LIGHT_TYPE type)
{
	mLightInfo.LightType = static_cast<int32>(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		mVolumeMesh = GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle");
		mLightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");
		mShadowCamera->GetCamera()->SetScale(1.f);
		mShadowCamera->GetCamera()->SetFar(10000.f);
		mShadowCamera->GetCamera()->SetWidth(4096);
		mShadowCamera->GetCamera()->SetHeight(4096);
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
