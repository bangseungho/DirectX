#pragma once
#include "Texture.h"
#include "Material.h"
#include "Camera.h"
#include "GameObject.h"

class Scene
{
public:
	void Awake();
	void Start();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	void Render();
	void RenderFinal();
	void RenderLights();
	void PushPassData();

public:
	void AddGameObject(sptr<GameObject> gameObject);
	void RemoveGameObject(sptr<GameObject> gameObject);

	const vector<sptr<GameObject>>& GetGameObjects() { return mGameObjects; }
	const vector<sptr<Camera>>& GetCameraObjects() { return mCameraObjects; }
	sptr<Camera>& GetMainCamera() { return mMainCamera; }

	void SetMainCamera(sptr<GameObject> camera) { mMainCamera = camera->GetCamera(); }

private:
	vector<sptr<GameObject>>			mGameObjects;
	Vec4								mAmbientLight = { 0.1f, 0.1f, 0.1f, 1.f };

	vector<sptr<Camera>>				mCameraObjects;
	sptr<Camera>						mMainCamera;

	vector<shared_ptr<class Light>>		mLightObjects;
};

