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

	const vector<sptr<GameObject>>& GetGameObjects() { return _gameObjects; }
	const vector<sptr<Camera>>& GetCameraObjects() { return _cameraObjects; }
	sptr<Camera>& GetMainCamera() { return _mainCamera; }

	void SetMainCamera(sptr<GameObject> camera) { _mainCamera = camera->GetCamera(); }

private:
	vector<sptr<GameObject>> _gameObjects;
	Vec4 _ambientLight = { 0.1f, 0.1f, 0.1f, 1.f };

	vector<sptr<Camera>> _cameraObjects;
	sptr<Camera> _mainCamera;

	vector<shared_ptr<class Light>> _lightObjects;
};

