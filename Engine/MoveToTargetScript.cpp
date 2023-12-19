#include "pch.h"
#include "MoveToTargetScript.h"
#include "Transform.h"

void MoveToTargetScript::LateUpdate()
{
	GetTransform()->SetLocalPosition(mTarget->GetTransform()->GetLocalPosition());
}
