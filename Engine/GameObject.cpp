#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "MonoBehaviour.h"
#include "Collider.h"

uint32 GameObject::ObjCBIndex = 0;

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{
	_objCBIndex = ObjCBIndex++;
}

GameObject::~GameObject()
{

}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->FixedUpdate();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->FinalUpdate();
	}
}

sptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

sptr<Transform> GameObject::GetTransform()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(_components[index]);
}

sptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(_components[index]);
}

sptr<Camera> GameObject::GetCamera()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(_components[index]);
}

sptr<Light> GameObject::GetLight()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::LIGHT);
	return static_pointer_cast<Light>(_components[index]);
}

sptr<Collider> GameObject::GetCollider()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::COLLIDER);
	return static_pointer_cast<Collider>(_components[index]);
}

void GameObject::AddComponent(sptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}