#pragma once
#include "MonoBehaviour.h"

class FlashLightScript : public MonoBehaviour
{
public:
	FlashLightScript() {};
	virtual ~FlashLightScript() {};

	void SetCameraObject(sptr<class GameObject> target) { mTarget = target; }

	virtual void Start() override;
	virtual void LateUpdate() override;

private:
	sptr<GameObject> mTarget;
};

