#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "Transform.h"
#include "Timer.h"

void Scene::Awake()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}

	if (_mainCamera != nullptr)
		_mainCamera->Awake();
}

void Scene::Render()
{
	PushPassData();

	_mainCamera->GetCamera()->Render();
}

void Scene::Start()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}

	if (_mainCamera != nullptr)
		_mainCamera->Start();
}

void Scene::FixedUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FixedUpdate();
	}

	if (_mainCamera != nullptr)
		_mainCamera->FixedUpdate();
}

void Scene::Update()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}

	if (_mainCamera != nullptr)
		_mainCamera->Update();
}

void Scene::LateUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}

	if (_mainCamera != nullptr)
		_mainCamera->LateUpdate();
}

void Scene::FinalUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}

	if (_mainCamera != nullptr)
		_mainCamera->FinalUpdate();
}

void Scene::AddGameObject(sptr<GameObject> gameObject)
{
	_gameObjects.push_back(gameObject);
}

void Scene::AddCameraObject(NUMBER_CAMERA number, sptr<GameObject> gameObject)
{
	_cameraObjects[number] = gameObject;
}

void Scene::RemoveGameObject(sptr<GameObject> gameObject)
{
	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}

void Scene::PushPassData()
{
	PassConstants passConstants = {};

	passConstants.view = _mainCamera->GetCamera()->_matView;
	passConstants.proj = _mainCamera->GetCamera()->_matProjection;
	passConstants.viewProj = passConstants.view * passConstants.proj;
	passConstants.eyePosW = _mainCamera->GetTransform()->GetLocalPosition();
	passConstants.nearZ = _mainCamera->GetCamera()->_near;
	passConstants.farZ = _mainCamera->GetCamera()->_far;
	passConstants.totalTime = TOTAL_TIME;
	passConstants.deltaTime = DELTA_TIME;
	passConstants.ambientLight = _ambientLight;

	for (auto& gameObject : _gameObjects)
	{
		if (gameObject->GetLight() == nullptr)
			continue;

		const LightInfo& lightInfo = gameObject->GetLight()->GetLightInfo();

		passConstants.lights[passConstants.lightCount] = lightInfo;
		passConstants.lightCount++;
	}

	CB(CONSTANT_BUFFER_TYPE::PASS)->CopyData(0, &passConstants, sizeof(passConstants));

}

void Scene::LoadTestTextures()
{
	auto newjeans = make_shared<Texture>();
	newjeans->Init(L"..\\Resources\\Texture\\newjeans.dds");
	_textures["newjeans"] = move(newjeans);
	DESCHEAP->Init(_textures.size());

	auto leather = make_shared<Texture>();
	leather->Init(L"..\\Resources\\Texture\\Leather.dds");
	_textures["leather"] = move(leather);
	DESCHEAP->Init(_textures.size());

	auto wall = make_shared<Texture>();
	wall->Init(L"..\\Resources\\Texture\\Sci-Fi_Wall_014_basecolor.dds");
	_textures["wall"] = move(wall);
	DESCHEAP->Init(_textures.size());

	for (auto& tex : _textures) {
		DESCHEAP->CreateSRV(tex.second);
	}
}

void Scene::BuildMaterials()
{
	{
		auto newjeans = make_shared<Material>();
		newjeans->SetMatCBIndex(0);
		newjeans->SetDiffuseSrvHeapIndex(0);
		newjeans->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans->SetRoughness(0.125f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		newjeans->SetShader(shader);

		_materials["newjeans"] = move(newjeans);
	}

	{
		auto leather = make_shared<Material>();
		leather->SetMatCBIndex(1);
		leather->SetDiffuseSrvHeapIndex(1);
		leather->SetFresnel(Vec3(0.01f, 0.01f, 0.01f));
		leather->SetRoughness(0.125f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		leather->SetShader(shader);

		_materials["leather"] = move(leather);
	}

	{
		auto wall = make_shared<Material>();
		wall->SetMatCBIndex(2);
		wall->SetDiffuseSrvHeapIndex(2);
		wall->SetFresnel(Vec3(0.01f, 0.01f, 0.01f));
		wall->SetRoughness(0.8f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		wall->SetShader(shader);

		_materials["wall"] = move(wall);
	}
}