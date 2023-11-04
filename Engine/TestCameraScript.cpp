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
	Vec3 Pos = GetTransform()->GetLocalPosition();

	if (KEY_PRESSED('W'))
		Pos += GetTransform()->GetLook() * mTranslationSpeed * DELTA_TIME;

	if (KEY_PRESSED('S'))
		Pos -= GetTransform()->GetLook() * mTranslationSpeed * DELTA_TIME;

	if (KEY_PRESSED('A'))
		Pos -= GetTransform()->GetRight() * mTranslationSpeed * DELTA_TIME;

	if (KEY_PRESSED('D'))
		Pos += GetTransform()->GetRight() * mTranslationSpeed * DELTA_TIME;

	if (KEY_PRESSED(VK_LEFT))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= mRotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_RIGHT))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += mRotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_UP))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= mRotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_PRESSED(VK_DOWN))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += mRotationSpeed * DELTA_TIME;
		GetTransform()->SetLocalRotation(rotation);
	}

	GetTransform()->SetLocalPosition(Pos);
}