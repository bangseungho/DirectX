#pragma once
#include "Component.h"

class Rigidbody3D : public Component
{
public:
	Rigidbody3D();
	virtual ~Rigidbody3D();

public:
	virtual void FixedUpdate() override;

public:
	void SetMass(float mass) { mMass = mass; }

private:
	float	mMass = 200.f;
	float	mDraw;
	float	mAngularDrag;

	bool	mUseGravity;
	bool	mIsKinematic;

	Vec3	mGravity;
	Vec3	mAccellaration;
	Vec3	mVelocity;
};
