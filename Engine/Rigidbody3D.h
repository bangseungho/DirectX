#pragma once
#include "Component.h"

class Rigidbody3D : public Component
{
public:
	Rigidbody3D();
	virtual ~Rigidbody3D();

public:
	virtual void FixedUpdate() override;

private:
	float _mass;
	float _drag;
	float _angularDrag;

	bool _useGravity;
	bool _isKinematic;
};
