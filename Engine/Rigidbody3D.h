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
	void SetMass(float mass) { _mass = mass; }

private:
	float _mass = 200.f;
	float _drag;
	float _angularDrag;

	bool _useGravity;
	bool _isKinematic;

	Vec3 _gravity;
	Vec3 _accellaration;;
	Vec3 _velocity;
};
