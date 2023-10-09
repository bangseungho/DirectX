#pragma once
#include "MonoBehaviour.h"

class TestAutoMoveScript : public MonoBehaviour
{
public:
	TestAutoMoveScript(float pivot);
	virtual ~TestAutoMoveScript();

	virtual void LateUpdate() override;

private:
	float _distance = 50.f;
	float _pivot = 0.f;
	float _moveSpeed = 3.f;
};