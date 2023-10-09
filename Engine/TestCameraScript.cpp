#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Timer.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (KEY_PRESSED('W'))
		pos += GetTransform()->GetLook() * _translationSpeed * DELTA_TIME;

	if (KEY_PRESSED('S'))
		pos -= GetTransform()->GetLook() * _translationSpeed * DELTA_TIME;

	if (KEY_PRESSED('A'))
		pos -= GetTransform()->GetRight() * _translationSpeed * DELTA_TIME;

	if (KEY_PRESSED('D'))
		pos += GetTransform()->GetRight() * _translationSpeed * DELTA_TIME;

	if (KEY_PRESSED(VK_LEFT))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= _rotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_RIGHT))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += _rotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_UP))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= _rotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_DOWN))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += _rotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	GetTransform()->SetLocalPosition(pos);
}