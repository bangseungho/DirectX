#include "pch.h"
#include "TestRotationScript.h"
#include "Transform.h"
#include "Timer.h"

TestRotationScript::TestRotationScript()
{
}

TestRotationScript::~TestRotationScript()
{
}

void TestRotationScript::LateUpdate()
{
	Vec3 rotation = GetTransform()->GetLocalRotation();
	rotation.y -= _rotationSpeed * DELTA_TIME;
	GetTransform()->SetLocalRotation(rotation);
}
