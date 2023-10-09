#pragma once

enum NUMBER_CAMERA
{
	FIRST_CAMERA,
	SECOND_CAMERA,
	THIRD_CAMERA,
	FOURTH_CAMERA,
	FIFTH_CAMERA,

	CAMERA_COUNT,
};

class GameObject;
class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void AddGameObject(sptr<GameObject> gameObject);
	void AddCameraObject(NUMBER_CAMERA number, sptr<GameObject> gameObject);

	void RemoveGameObject(sptr<GameObject> gameObject);

	const vector<sptr<GameObject>>& GetGameObjects() { return _gameObjects; }

	void SetMainCamera(NUMBER_CAMERA cameraNum) { if (_cameraObjects[cameraNum]) _mainCamera = _cameraObjects[cameraNum]; }
	const sptr<GameObject>& GetMainCamera() { return _mainCamera; }

private:
	vector<sptr<GameObject>> _gameObjects;
	
	array<sptr<GameObject>, CAMERA_COUNT> _cameraObjects;
	sptr<GameObject> _mainCamera;
};
