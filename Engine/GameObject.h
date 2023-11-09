#pragma once
#include "Component.h"
#include "Object.h"
#include "Collider.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;
class Collider;
class ParticleSystem;
class Terrain;
class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

public:
	void Init();

public:
	void Awake();
	void Start();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	sptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	sptr<Transform> GetTransform();
	sptr<MeshRenderer> GetMeshRenderer();
	sptr<Camera> GetCamera();
	sptr<Light> GetLight();
	sptr<Collider> GetCollider();
	sptr<ParticleSystem> GetParticleSystem();
	sptr<Terrain> GetTerrain();
	
 	void AddComponent(sptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { mCheckFrustum = checkFrustum; }
	bool GetCheckFrustum() { return mCheckFrustum; }

	void SetObjCBIndex(uint32 index) { mObjectIndex = index; }
	uint32 GetObjCBIndex() const { return mObjectIndex; }

	void SetMatIndex(uint32 index) { mMatIndex = index; }
	uint32 GetMatIndex() const { return mMatIndex; }

	void SetLayerIndex(uint8 layer) { mLayerIndex = layer; }
	uint8 GetLayerIndex() { return mLayerIndex; }

	void SetActive(bool active) { mActiveSelf = active; }
	bool GetActive() const { return mActiveSelf; }

	static uint32 ObjCBIndex;

private:
	array<sptr<Component>, FIXED_COMPONENT_COUNT>	mComponents;
	vector<sptr<MonoBehaviour>>						mScripts;

	bool		mCheckFrustum = true;
	uint32		mObjectIndex = -1;
	uint32		mMatIndex = -1;

	uint8		mLayerIndex = 0;

	bool		mActiveSelf = true;
};

