#pragma once
#include "MonoBehaviour.h"

class GunPlayerScript : public MonoBehaviour
{
public:
	GunPlayerScript();
	virtual ~GunPlayerScript() {};

	void SetCameraObject(sptr<GameObject> target) { mTarget = target; }

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	vector<sptr<GameObject>>& GetGunParts() { return mGunParts; }
	vector<sptr<GameObject>>& GetBullets() { return mBullets; }

private:
	Vec3 mPivotPosition;
	float mAcc = 0.f;
	float mIdleBreathSpeed = 2.f;
	float mWorkBreathSpeed = 10.f;
	float mBreathLength = 60.f;

	sptr<GameObject> mTarget;
	vector<sptr<GameObject>> mGunParts;
	vector<sptr<GameObject>> mBullets;
	float mShotDelay = 3.f;
};

