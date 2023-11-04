#pragma once
#include "MonoBehaviour.h"

class TestRotationScript : public MonoBehaviour
{
public:
	TestRotationScript();
	virtual ~TestRotationScript();

	virtual void LateUpdate() override;

private:
	float mRotationSpeed = 2.f;
};

