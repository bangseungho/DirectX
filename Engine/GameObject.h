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
	
 	void AddComponent(sptr<Component> component);
	void SetTag(wstring tag) { _tag = tag; }
	void SetMinMaxVertices(MinMaxVert minMaxVertices) { _minMaxVertices = minMaxVertices; }
	MinMaxVert GetMinMaxVertices() { return _minMaxVertices; }

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetObjCBIndex(uint32 index) { _objCBIndex = index; }
	uint32 GetObjCBIndex() const { return _objCBIndex; }

	void SetMatIndex(uint32 index) { _matIndex = index; }
	uint32 GetMatIndex() const { return _matIndex; }

	static uint32 ObjCBIndex;

private:
	array<sptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<sptr<MonoBehaviour>> _scripts;
	wstring _tag;
	MinMaxVert _minMaxVertices;

	bool _checkFrustum = true;
	uint32 _objCBIndex = -1;
	uint32 _matIndex = -1;
};

