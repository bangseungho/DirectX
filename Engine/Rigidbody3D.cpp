#include "pch.h"
#include "Rigidbody3D.h"
#include "Transform.h"
#include "Timer.h"

Rigidbody3D::Rigidbody3D() : Component(COMPONENT_TYPE::RIGIDBODY3D)
{
	_gravity = Vec3(0.f, -9.8f, 0.f);
	_accellaration = _gravity;
	_velocity = Vec3(0.f, 0.f, 0.f);
}

Rigidbody3D::~Rigidbody3D()
{
}

void Rigidbody3D::FixedUpdate()
{
	Vec3 localPos = GetTransform()->GetLocalPosition();
	localPos += _velocity * DELTA_TIME;
	GetTransform()->SetLocalPosition(localPos);
	_velocity += _accellaration * _mass * DELTA_TIME;

	if (localPos.y <= 100.f)
		_velocity.y = 500.f;
}
