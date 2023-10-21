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
	bool IsInFrustum(BoundingOrientedBox& boundsOOBB);

	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE GetProjectionType() const { return _type; }

	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

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
	
	uint32 _cullingMask = 0;

public:
	static Matrix MatView;
	static Matrix MatProjection;
};


