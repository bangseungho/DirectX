#include "pch.h"
#include "Rigidbody3D.h"
#include "Transform.h"
#include "Timer.h"

Rigidbody3D::Rigidbody3D() : Component(COMPONENT_TYPE::RIGIDBODY3D)
{
	mGravity = Vec3(0.f, -9.8f, 0.f);
	mAccellaration = mGravity;
	mVelocity = Vec3(0.f, 0.f, 0.f);

	mUseGravity = false;
}

Rigidbody3D::~Rigidbody3D()
{
}

void Rigidbody3D::FixedUpdate()
{
	Vec3 localPos = GetTransform()->GetLocalPosition();

	if (!mUseGravity)
		return;

	localPos += mVelocity * DELTA_TIME;
	GetTransform()->SetLocalPosition(localPos);
	mVelocity += mAccellaration * mMass * DELTA_TIME;

	if (localPos.y <= 100.f)
		mVelocity.y = 500.f;
}
