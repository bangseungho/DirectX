#pragma once
#include "MonoBehaviour.h"

class TestScript : public MonoBehaviour
{
public:
	TestScript();
	virtual ~TestScript();

	virtual void LateUpdate() override;

private:
	float mRotationSpeed = 2.f;
};
