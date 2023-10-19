#pragma once
#include "Component.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

public:
	virtual void FinalUpdate() override;

public:
	void Render();
	void GenerateFrustum();
	bool IsInFrustum(BoundingOrientedBox& boundsOOBB);

private:
	friend class Scene;

	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 10000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	Matrix _matView = {};
	Matrix _matProjection = {};

	BoundingFrustum _frustum;
};


