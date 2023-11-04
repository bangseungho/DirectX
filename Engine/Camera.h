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
	bool IsInFrustum(BoundingOrientedBox& boundsOOBB);

	void SetProjectionType(PROJECTION_TYPE type) { mProjectionType = type; }
	PROJECTION_TYPE GetProjectionType() const { return mProjectionType; }

	void SortGameObject();
	void Render_Deferred();
	void Render_Forward();
	//void Render_Transparent();

	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			mCullingMask |= (1 << layer);
		else
			mCullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { mCullingMask = mask; }
	bool IsCulled(uint8 layer) { return (mCullingMask & (1 << layer)) != 0; }

private:
	friend class Scene;

	PROJECTION_TYPE		mProjectionType = PROJECTION_TYPE::PERSPECTIVE;
	float				mNear = 1.f;
	float				mFar = 2000.f;
	float				mFov = XM_PI / 4.f;
	float				mScale = 1.0f;
	Matrix				mMatView = {};
	Matrix				mMatProjection = {};
	BoundingFrustum		mFrustum;
	uint32				mCullingMask = 0;

private:
	vector<shared_ptr<GameObject>>	mDeferredObjects;
	vector<shared_ptr<GameObject>>	mForwardObjects;
	vector<shared_ptr<GameObject>>	mParticleObjects;

public:
	static Matrix sMatView;
	static Matrix sMatProjection;
};


