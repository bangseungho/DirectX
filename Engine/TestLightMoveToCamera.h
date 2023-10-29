#pragma once
#include "MonoBehaviour.h"

class TestLightMoveToCamera : public MonoBehaviour
{
public:
	TestLightMoveToCamera() {};
	virtual ~TestLightMoveToCamera() {};

	void SetGameObject(sptr<class Camera> target) { _target = target; }

	virtual void LateUpdate() override;

private:
	sptr<class Camera> _target;
};

