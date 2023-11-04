#include "pch.h"
#include "TestAutoMoveScript.h"
#include "Transform.h"
#include "Timer.h"

TestAutoMoveScript::TestAutoMoveScript(float pivot)
{
	mPivot = pivot;
}

TestAutoMoveScript::~TestAutoMoveScript()
{
}

void TestAutoMoveScript::LateUpdate()
{
	Vec3 Pos = GetTransform()->GetLocalPosition();
	

	Pos.x = mPivot + sinf(TOTAL_TIME * mMoveSpeed) * mDistance;


	GetTransform()->SetLocalPosition(Pos);
}
