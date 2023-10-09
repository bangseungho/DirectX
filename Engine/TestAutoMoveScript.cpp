#include "pch.h"
#include "TestAutoMoveScript.h"
#include "Transform.h"
#include "Timer.h"

TestAutoMoveScript::TestAutoMoveScript(float pivot)
{
	_pivot = pivot;
}

TestAutoMoveScript::~TestAutoMoveScript()
{
}

void TestAutoMoveScript::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	

	pos.x = _pivot + sinf(TOTAL_TIME * _moveSpeed) * _distance;


	GetTransform()->SetLocalPosition(pos);
}
