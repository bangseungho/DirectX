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
	void SortShadowObject();

	void Render_Deferred();
	void Render_Forward();
	void Render_Shadow();

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

	void SetNear(float value) { mNear = value; }
	void SetFar(float value) { mFar = value; }
	void SetFOV(float value) { mFov = value; }
	void SetScale(float value) { mScale = value; }
	void SetWidth(float value) { mWidth = value; }
	void SetHeight(float value) { mHeight = value; }

	Matrix& GetViewMatrix() { return mMatView; }
	Matrix& GetProjectionMatrix() { return mMatProjection; }
	Matrix& GetMatShadowViewProj() { return mMatShadowViewProj; }

private:
	friend class Scene;

	PROJECTION_TYPE		mProjectionType = PROJECTION_TYPE::PERSPECTIVE;
	float				mNear = 1.f;
	float				mFar = 10000.f;
	float				mFov = XM_PI / 4.f;
	float				mScale = 1.0f;
	Matrix				mMatView = {};
	Matrix				mMatProjection = {};
	Matrix				mMatShadowViewProj = {};
	BoundingFrustum		mFrustum;
	uint32				mCullingMask = 0;

	float				mWidth = 0.f;
	float				mHeight = 0.f;

private:
	vector<shared_ptr<GameObject>>	mDeferredObjects;
	vector<shared_ptr<GameObject>>	mForwardObjects;
	vector<shared_ptr<GameObject>>	mParticleObjects;
	vector<shared_ptr<GameObject>>	mShadowObjects;

public:
	static Matrix sMatView;
	static Matrix sMatProjection;
	static Matrix sMatShadowViewProj;
};


