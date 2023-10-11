#pragma once
#include "MonoBehaviour.h"

class TestLightMoveToCamera : public MonoBehaviour
{
public:
	TestLightMoveToCamera() {};
	virtual ~TestLightMoveToCamera() {};

	void SetGameObject(sptr<class GameObject> target) { _target = target; }

	virtual void LateUpdate() override;

private:
	sptr<class GameObject> _target;
};
