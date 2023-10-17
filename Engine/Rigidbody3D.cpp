#include "pch.h"
#include "Rigidbody3D.h"
#include "Transform.h"

Rigidbody3D::Rigidbody3D() : Component(COMPONENT_TYPE::RIGIDBODY3D)
{
}

Rigidbody3D::~Rigidbody3D()
{
}

void Rigidbody3D::FixedUpdate()
{
	Vec3 localPos = GetTransform()->GetLocalPosition();
	
	localPos.y -= 1.0f;
	
	GetTransform()->SetLocalPosition(localPos);
}
