#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();
	
public:
	virtual void Start() override;
	virtual void Update() override;

private:
	Vec3 _center;
	bool _isTrigger;
};

