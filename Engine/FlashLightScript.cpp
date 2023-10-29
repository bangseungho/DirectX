#include "pch.h"
#include "FlashLightScript.h"
#include "GameObject.h"
#include "Light.h"
#include "Transform.h"
#include "Camera.h"

void FlashLightScript::LateUpdate()
{
	Vec3 targetPos = _target->GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(targetPos);

	Vec3 targetDir = _target->GetTransform()->GetLook();
	GetGameObject()->GetLight()->SetLightDirection(targetDir);
}
