#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(COMPONENT_TYPE type) : Object(OBJECT_TYPE::COMPONENT), mType(type)
{

}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
	return mGameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
	return mGameObject.lock()->GetTransform();
}
