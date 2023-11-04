#pragma once
#include "MonoBehaviour.h"

class TestLightMoveToCamera : public MonoBehaviour
{
public:
	TestLightMoveToCamera() {};
	virtual ~TestLightMoveToCamera() {};

	void SetGameObject(sptr<class Camera> target) { mTarget = target; }

	virtual void LateUpdate() override;

private:
	sptr<class Camera> mTarget;
};

