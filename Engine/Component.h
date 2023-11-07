#pragma once
#include "Object.h"

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	RIGIDBODY3D,
	COLLIDER,
	PARTICLE_SYSTEM,
	TERRAIN,

	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;
class Component : public Object
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	virtual void Awake() { }
	virtual void Start() { }
	virtual void FixedUpdate() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

public:
	COMPONENT_TYPE GetType() { return mType; }
	bool IsValid() { return mGameObject.expired() == false; }

	sptr<GameObject> GetGameObject();
	sptr<Transform> GetTransform();

private:
	friend class GameObject;
	void SetGameObject(sptr<GameObject> gameObject) { mGameObject = gameObject; }

protected:
	COMPONENT_TYPE		mType;
	wptr<GameObject>	mGameObject;
};

