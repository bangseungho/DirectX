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

	PASS_CB->CopyData(0, passConstants);
}

void Scene::LoadTestTextures()
{
	vector<string> texNames = {
		"newjeans",
		"newjeans3",

		"leather",
		"leather_normal",
		"leather_roughness",

		"wall",
		"wall_normal",
		"wall_roughness",
	};

	vector<wstring> texFileNames = {
		L"..\\Resources\\Texture\\newjeans.dds",
		L"..\\Resources\\Texture\\newjeans3.dds",

		L"..\\Resources\\Texture\\Leather.dds",
		L"..\\Resources\\Texture\\Leather_Normal_BC7.dds",
		L"..\\Resources\\Texture\\Leather_Weave_004_roughness.dds",

		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_basecolor.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_normal_BC7.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_roughness.dds",
	};

	// texture2D
	for (int i = 0; i < TEXTURE2D_COUNT; ++i) {
		auto texMap = std::make_shared<Texture>();
		texMap->Init(texFileNames[i]);
		DESCHEAP->CreateSRV(texMap);
		_textures[texNames[i]] = move(texMap);
	}

	// textureCube
	auto skyMap = std::make_shared<Texture>();
	skyMap->Init(L"..\\Resources\\Texture\\Sky.dds");
	DESCHEAP->CreateSRV(skyMap, TEXTURE_TYPE::TEXTURECUBE);
	_textures["skybox"] = move(skyMap);
}

void Scene::BuildMaterials()
{
	{
		auto newjeans = make_shared<Material>();
		newjeans->SetMatCBIndex(0);
		newjeans->SetDiffuseSrvHeapIndex(TEXTURE2D_INDEX::B_NEWJEANS);
		newjeans->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans->SetRoughness(0.125f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		newjeans->SetShader(shader);

		_materials["newjeans"] = move(newjeans);
	}

	{
		auto newjeans3 = make_shared<Material>();
		newjeans3->SetMatCBIndex(1);
		newjeans3->SetDiffuseSrvHeapIndex(TEXTURE2D_INDEX::B_NEWJEANS3);
		newjeans3->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans3->SetRoughness(0.125f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		newjeans3->SetShader(shader);

		_materials["newjeans3"] = move(newjeans3);
	}

	{
		auto leather = make_shared<Material>();
		leather->SetMatCBIndex(2);
		leather->SetDiffuseSrvHeapIndex(TEXTURE2D_INDEX::B_LEATHER);
		leather->SetNormalSrvHeapIndex(TEXTURE2D_INDEX::N_LEATHER);
		leather->SetRoughnessSrvHeapIndex(TEXTURE2D_INDEX::R_LEATHER);
		leather->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		leather->SetRoughness(0.125f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		leather->SetShader(shader);

		_materials["leather"] = move(leather);
	}

	{
		auto wall = make_shared<Material>();
		wall->SetMatCBIndex(3);
		wall->SetDiffuseSrvHeapIndex(TEXTURE2D_INDEX::B_WALL);
		wall->SetNormalSrvHeapIndex(TEXTURE2D_INDEX::N_WALL);
		wall->SetRoughnessSrvHeapIndex(TEXTURE2D_INDEX::R_WALL);
		wall->SetFresnel(Vec3(0.5f, 0.5f, 0.5f));
		wall->SetRoughness(0.5f);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
		wall->SetShader(shader);

		_materials["wall"] = move(wall);
	}

	{
		auto skybox = make_shared<Material>();
		skybox->SetMatCBIndex(4);
		skybox->SetDiffuseSrvHeapIndex(TEXTURECUBE_INDEX::SKYBOX);
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Sky.hlsl", { RASTERIGER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
		skybox->SetShader(shader);

		_materials["skybox"] = move(skybox);
	}
}