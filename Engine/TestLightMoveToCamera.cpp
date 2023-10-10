#include "pch.h"
#include "TestLightMoveToCamera.h"
#include "Transform.h"
#include "GameObject.h"

void TestLightMoveToCamera::LateUpdate()
{
	Vec3 targetPos = _target->GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(targetPos);
}
