#pragma once
#include "MonoBehaviour.h"

class FlashLightScript : public MonoBehaviour
{
public:
	FlashLightScript() {};
	virtual ~FlashLightScript() {};

	void SetCameraObject(sptr<class Camera> target) { mTarget = target; }

	virtual void LateUpdate() override;

private:
	sptr<class Camera> mTarget;
};

