#pragma once
#include "MonoBehaviour.h"

class TestAutoMoveScript : public MonoBehaviour
{
public:
	TestAutoMoveScript(float pivot);
	virtual ~TestAutoMoveScript();

	virtual void LateUpdate() override;

private:
	float mDistance = 50.f;
	float mPivot = 0.f;
	float mMoveSpeed = 3.f;
};