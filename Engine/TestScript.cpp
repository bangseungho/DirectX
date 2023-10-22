#include "pch.h"
#include "TestScript.h"
#include "Transform.h"
#include "Timer.h"

TestScript::TestScript()
{
}

TestScript::~TestScript()
{
}

void TestScript::LateUpdate()
{
	float x = GetTransform()->GetLocalScale().x;
	float y = GetTransform()->GetLocalScale().y;

	x -= 0.05f * DELTA_TIME;
	y -= 0.05f * DELTA_TIME;

	GetTransform()->GetTransform()->SetLocalScale(Vec3(x, y, 1.f));
}
