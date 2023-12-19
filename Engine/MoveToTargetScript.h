#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"

class MoveToTargetScript : public MonoBehaviour
{
public:
	virtual void LateUpdate() override;

	void SetTargetObject(sptr<GameObject> target) { mTarget = target; }

private:
	sptr<GameObject> mTarget;
};

