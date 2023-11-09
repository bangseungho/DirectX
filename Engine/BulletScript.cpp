#include "pch.h"
#include "BulletScript.h"
#include "Transform.h"
#include "GameObject.h"
#include "Timer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "Resources.h"
#include "ScoreScript.h"

BulletScript::BulletScript()
{
}

BulletScript::~BulletScript()
{
}

void BulletScript::LateUpdate()
{
	mLifeTime += DELTA_TIME;

	Vec3 localPosition = GetTransform()->GetLocalPosition();
	localPosition += GetTransform()->GetLook() * mSpeed * DELTA_TIME;

	GetTransform()->SetLocalPosition(localPosition);

	if (mLifeTime >= mRange) {
		GetGameObject()->SetActive(false);
		mLifeTime = 0.f;
	}

	auto activeScene = GET_SINGLE(SceneManager)->GetActiveScene();
	auto gameObjects = activeScene->GetGameObjects();
	auto particleObjects = activeScene->GetParticleObjects();
	
	for (auto& gameObject : gameObjects) {
		if (gameObject->GetActive() == false)
			continue;

		if (gameObject->GetCollider() == nullptr)
			continue;

		BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
		boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());

		BoundingOrientedBox myBoundingBox = GetGameObject()->GetMeshRenderer()->GetBoundingBox();
		myBoundingBox.Transform(myBoundingBox, GetTransform()->GetLocalToWorldMatrix());

		if (myBoundingBox.Intersects(boundingBox)) {
			mLifeTime = 0.f;
			gameObject->SetActive(false);
			GetGameObject()->SetActive(false);

			auto scoreComponent = GET_SINGLE(Resources)->Get<Component>(L"ScoreScript");
			auto scoreScript = dynamic_pointer_cast<ScoreScript>(scoreComponent);
			scoreScript->AddScore();

			for (int i = 0; i < particleObjects.size(); ++i) {
				if (particleObjects[i]->GetActive() == false) {
					particleObjects[i]->SetActive(true);
					particleObjects[i]->GetTransform()->SetLocalPosition(localPosition);
					break;
				}
			}

		}
	}



}

