#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent ±‚¡ÿ
	const Vec3& GetLocalPosition() { return mLocalPosition; }
	const Vec3& GetLocalRotation() { return mLocalRotation; }
	const Vec3& GetLocalScale() { return mLocalScale; }

	// TEMP
	float GetBoundingSphereRadius() { return max(max(mLocalScale.x, mLocalScale.y), mLocalScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return mMatWorld; }
	Vec3 GetWorldPosition() { return mMatWorld.Translation(); }

	void SetTexTransform(Matrix transform) { mTexTransform = transform; }
	Matrix& GetTexTransform() { return mTexTransform; }

	Vec3 GetRight() { return mMatWorld.Right(); }
	Vec3 GetUp() { return mMatWorld.Up(); }
	Vec3 GetLook() { return mMatWorld.Backward(); }

	void SetLocalPosition(const Vec3& Position) { mLocalPosition = Position; }
	void SetLocalRotation(const Vec3& rotation) { mLocalRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { mLocalScale = scale; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
	void SetParent(shared_ptr<Transform> parent) { mParent = parent; }
	weak_ptr<Transform> GetParent() { return mParent; }

private:
	Vec3					mLocalPosition = {};
	Vec3					mLocalRotation = {};
	Vec3					mLocalScale = { 1.f, 1.f, 1.f };

	Matrix					mMatLocal = {};
	Matrix					mMatWorld = {};
	Matrix					mTexTransform = {};

	weak_ptr<Transform>		mParent;
};

