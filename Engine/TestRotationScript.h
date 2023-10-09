#pragma once
#include "MonoBehaviour.h"

class TestRotationScript : public MonoBehaviour
{
public:
	TestRotationScript();
	virtual ~TestRotationScript();

	virtual void LateUpdate() override;

private:
	float _rotationSpeed = 2.f;
};

