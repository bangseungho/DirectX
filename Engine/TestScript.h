#pragma once
#include "MonoBehaviour.h"

class TestScript : public MonoBehaviour
{
public:
	TestScript();
	virtual ~TestScript();

	virtual void LateUpdate() override;

private:
	float _rotationSpeed = 2.f;
};
