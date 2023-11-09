#include "pch.h"
#include "GunPlayerScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Resources.h"

GunPlayerScript::GunPlayerScript()
{

}

void GunPlayerScript::Start()
{
	GetTransform()->SetParent(mTarget->GetTransform());
	GetTransform()->SetLocalScale(Vec3(8.f, 8.f, 8.f));
	GetTransform()->SetLocalRotation(Vec3(0.f, -90.f, 0.f));
	GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 30.f));
	mPivotPosition = GetTransform()->GetLocalPosition();

	for (auto& gunPart : mGunParts) {
		gunPart->GetTransform()->SetParent(GetTransform());
	}

	Vec3 scopePosition = mGunParts[0]->GetTransform()->GetLocalPosition();
	scopePosition.y += 0.8f;
	mGunParts[0]->GetTransform()->SetLocalPosition(scopePosition);

	Vec3 handlePosition = mGunParts[1]->GetTransform()->GetLocalPosition();
	handlePosition.x += 3.f;
	mGunParts[1]->GetTransform()->SetLocalPosition(handlePosition);

	Vec3 rePosition = mGunParts[2]->GetTransform()->GetLocalPosition();
	rePosition.x += 1.f;
	rePosition.y += 0.3f;
	mGunParts[2]->GetTransform()->SetLocalPosition(rePosition);
}

void GunPlayerScript::Update()
{
	float breathSpeed = mIdleBreathSpeed;

	if (KEY_PRESSED('W') ||
		KEY_PRESSED('S') ||
		KEY_PRESSED('A') ||
		KEY_PRESSED('D'))
		breathSpeed = mWorkBreathSpeed;

	mAcc += DELTA_TIME * breathSpeed;
	mShotDelay += DELTA_TIME;

	if (KEY_PRESSED(VK_CONTROL) && mShotDelay >= 0.1f) {
		sptr<GameObject> bullet = nullptr;
		for (int i = 0; i < mBullets.size(); ++i) {
			if (!mBullets[i]->GetActive()) {
				bullet = mBullets[i];
				bullet->SetActive(true);

				Vec3 o = GetTransform()->GetLocalToWorldMatrix().Translation();
				o += mTarget->GetTransform()->GetLook() * 80;
				o += mTarget->GetTransform()->GetRight() * 3;
				o += mTarget->GetTransform()->GetUp() * 2;

				bullet->GetTransform()->SetLocalPosition(o);
				bullet->GetTransform()->SetLocalRotation(mTarget->GetTransform()->GetLocalRotation());

				mShotDelay = 0.f;
				break;
			}
		}
	}
}

void GunPlayerScript::LateUpdate()
{	
	Vec3 localPosition;
	localPosition.y = sin(mAcc);

	GetTransform()->SetLocalPosition(mPivotPosition + localPosition);
}
