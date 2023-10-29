#include "pch.h"
#include "TestLightMoveToCamera.h"
#include "Transform.h"
#include "Camera.h"

void TestLightMoveToCamera::LateUpdate()
{
	Vec3 targetPos = _target->GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(targetPos);
}
