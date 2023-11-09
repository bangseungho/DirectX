#include "pch.h"
#include "FlashLightScript.h"
#include "GameObject.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"

void FlashLightScript::Start()
{
	GetTransform()->SetParent(mTarget->GetTransform());
}

void FlashLightScript::LateUpdate()
{
	Vec3 targetDir = mTarget->GetTransform()->GetLook();
	GetGameObject()->GetLight()->SetLightDirection(targetDir);
}
