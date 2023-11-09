#pragma once
#include "MonoBehaviour.h"

class BulletScript : public MonoBehaviour
{
public:
	BulletScript();
	virtual ~BulletScript();

	virtual void LateUpdate() override;

	void SetDirection(Vec3 direction) { mDirection = direction; }

private:
	float mSpeed = 500.f;
	Vec3 mDirection = { 1.f, 1.f, 1.f };

	sptr<GameObject> mPlayer;
	float mLifeTime = 0.f;
	float mRange = 5.f;
};

