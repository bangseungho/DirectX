#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class MonoBehaviour;

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
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	sptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	sptr<Transform> GetTransform();
	sptr<MeshRenderer> GetMeshRenderer();
	sptr<Camera> GetCamera();

	void AddComponent(sptr<Component> component);

private:
	array<sptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<sptr<MonoBehaviour>> _scripts;
};

