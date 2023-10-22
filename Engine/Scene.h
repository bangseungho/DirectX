#pragma once
#include "Texture.h"
#include "Material.h"

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
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();

private:
	void PushPassData();

public:
	void AddGameObject(sptr<GameObject> gameObject);
	void AddCameraObject(NUMBER_CAMERA number, sptr<GameObject> gameObject);

	void RemoveGameObject(sptr<GameObject> gameObject);

	const vector<sptr<GameObject>>& GetGameObjects() { return _gameObjects; }

	void SetMainCamera(NUMBER_CAMERA cameraNum) { if (_cameraObjects[cameraNum]) _mainCamera = _cameraObjects[cameraNum]; }
	const sptr<GameObject>& GetMainCamera() { return _mainCamera; }

	unordered_map<string, sptr<Texture>>& GetTextures() { return _textures; }
	unordered_map<string, sptr<Material>>& GetMaterials() { return _materials; }

	void LoadTestTextures();
	void LoadTestTexturesFromResource();
	void BuildMaterials();

private:
	vector<sptr<GameObject>> _gameObjects;
	array<sptr<GameObject>, CAMERA_COUNT> _cameraObjects;
	Vec4 _ambientLight = { 0.1f, 0.1f, 0.1f, 1.f };
	sptr<GameObject> _mainCamera;

	//unordered_map<string, sptr<class Material>> _materials;
	unordered_map<string, sptr<Texture>> _textures;
	unordered_map<string, sptr<Material>> _materials;
};

