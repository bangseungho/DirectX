#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "UploadBuffer.h"
#include "Light.h"
#include "Transform.h"
#include "Timer.h"
#include "Resources.h"

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

	int8 backIndex = gEngine->GetSwapChain()->GetBackBufferIndex();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();

	_mainCamera->GetCamera()->SortGameObject();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	_mainCamera->GetCamera()->Render_Deferred();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
	_mainCamera->GetCamera()->Render_Forward();

	// UI Camera 
	for (auto& gameObject : _gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->SortGameObject();

		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
		gameObject->GetCamera()->Render_Forward();
	}
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

void Scene::LoadTestTexturesFromResource()
{
	vector<string> texNames = {
		"PositionTarget",
		"NormalTarget",
		"DiffuseTarget",
		"FresnelTarget",
		"ShininessTarget",
	};

	for (int i = 0; i < RENDER_TARGET_G_BUFFER_GROUP_COUNT; ++i) {
		auto texMap = GET_SINGLE(Resources)->Get<Texture>(texNames[i]);
		_textures[texNames[i]] = texMap->GetTexHeapIndex();
	}
}

void Scene::LoadTestTextures()
{
	vector<string> texNames = {
		"newjeans",
		"newjeans2",
		"newjeans3",

		"leather",
		"leather_normal",
		"leather_roughness",

		"wall",
		"wall_normal",
		"wall_roughness",

		"skybox",
	};

	vector<wstring> texFileNames = {
		L"..\\Resources\\Texture\\newjeans.dds",
		L"..\\Resources\\Texture\\newjeans2.dds",
		L"..\\Resources\\Texture\\newjeans3.dds",

		L"..\\Resources\\Texture\\Leather.dds",
		L"..\\Resources\\Texture\\Leather_Normal_BC7.dds",
		L"..\\Resources\\Texture\\Leather_Weave_004_roughness.dds",

		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_basecolor.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_normal_BC7.dds",
		L"..\\Resources\\Texture\\Sci-Fi_Wall_014_roughness.dds",

		L"..\\Resources\\Texture\\Sky.dds",
	};

	for (int i = 0; i < TEXTUREFILE_COUNT; ++i) {
		auto texMap = GET_SINGLE(Resources)->Load<Texture>(texNames[i], texFileNames[i]);

		if (i == (TEXTUREFILE_COUNT - 1))
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURECUBE);
		else 
			texMap->CreateSRVFromDescHeap(TEXTURE_TYPE::TEXTURE2D);

		_textures[texNames[i]] = texMap->GetTexHeapIndex();
	}
}

void Scene::BuildMaterials()
{
	{
		auto pos = make_shared<Material>();
		pos->SetMatCBIndex(0);
		pos->SetDiffuseSrvHeapIndex(_textures["PositionTarget"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
		pos->SetShader(shader);
		_materials["position"] = move(pos);
	}

	{
		auto norm = make_shared<Material>();
		norm->SetMatCBIndex(1);
		norm->SetDiffuseSrvHeapIndex(_textures["NormalTarget"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
		norm->SetShader(shader);
		_materials["normal"] = move(norm);
	}

	{
		auto diffuse = make_shared<Material>();
		diffuse->SetMatCBIndex(2);
		diffuse->SetDiffuseSrvHeapIndex(_textures["DiffuseTarget"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
		diffuse->SetShader(shader);
		_materials["diffuse"] = move(diffuse);
	}

	{
		auto fresnel = make_shared<Material>();
		fresnel->SetMatCBIndex(3);
		fresnel->SetDiffuseSrvHeapIndex(_textures["FresnelTarget"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
		fresnel->SetShader(shader);
		_materials["fresnel"] = move(fresnel);
	}

	{
		auto shininess = make_shared<Material>();
		shininess->SetMatCBIndex(4);
		shininess->SetDiffuseSrvHeapIndex(_textures["ShininessTarget"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
		shininess->SetShader(shader);
		_materials["shininess"] = move(shininess);
	}

	{
		auto newjeans = make_shared<Material>();
		newjeans->SetMatCBIndex(5);
		newjeans->SetDiffuseSrvHeapIndex(_textures["newjeans"]);
		newjeans->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans->SetRoughness(0.125f);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Deferred");
		newjeans->SetShader(shader);

		_materials["newjeans"] = move(newjeans);
	}

	{
		auto newjeans2 = make_shared<Material>();
		newjeans2->SetMatCBIndex(6);
		newjeans2->SetDiffuseSrvHeapIndex(_textures["newjeans2"]);
		newjeans2->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		newjeans2->SetRoughness(0.125f);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Deferred");
		newjeans2->SetShader(shader);

		_materials["newjeans2"] = move(newjeans2);
	}

	{
		auto newjeans3 = make_shared<Material>();
		newjeans3->SetMatCBIndex(7);
		newjeans3->SetDiffuseSrvHeapIndex(_textures["newjeans3"]);
		newjeans3->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
		newjeans3->SetRoughness(0.125f);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Deferred");
		newjeans3->SetShader(shader);

		_materials["newjeans3"] = move(newjeans3);
	}

	{
		auto leather = make_shared<Material>();
		leather->SetMatCBIndex(8);
		leather->SetDiffuseSrvHeapIndex(_textures["leather"]);
		leather->SetNormalSrvHeapIndex(_textures["leather_normal"]);
		leather->SetRoughnessSrvHeapIndex(_textures["leather_roughness"]);
		leather->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
		leather->SetRoughness(0.125f);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Deferred");
		leather->SetShader(shader);

		_materials["leather"] = move(leather);
	}

	{
		auto wall = make_shared<Material>();
		wall->SetMatCBIndex(9);
		wall->SetDiffuseSrvHeapIndex(_textures["wall"]);
		wall->SetNormalSrvHeapIndex(_textures["wall_normal"]);
		wall->SetRoughnessSrvHeapIndex(_textures["wall_roughness"]);
		wall->SetFresnel(Vec3(0.5f, 0.5f, 0.5f));
		wall->SetRoughness(0.5f);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Deferred");
		wall->SetShader(shader);

		_materials["wall"] = move(wall);
	}

	{
		auto skybox = make_shared<Material>();
		skybox->SetMatCBIndex(10);
		skybox->SetDiffuseSrvHeapIndex(_textures["skybox"]);
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("SkyBox");
		skybox->SetShader(shader);

		_materials["skybox"] = move(skybox);
	}


	//{
	//	auto normal = make_shared<Material>();
	//	normal->SetMatCBIndex(7);
	//	normal->SetDiffuseSrvHeapIndex(GBUFFER_TEXTURE_INDEX::G_NORMAL);
	//	shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
	//	normal->SetShader(shader);

	//	_materials["normal"] = move(normal);
	//}

	//{
	//	auto diffuseAlbedo = make_shared<Material>();
	//	diffuseAlbedo->SetMatCBIndex(8);
	//	diffuseAlbedo->SetDiffuseSrvHeapIndex(GBUFFER_TEXTURE_INDEX::G_DIFFUSEALBEDO);
	//	shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
	//	diffuseAlbedo->SetShader(shader);

	//	_materials["diffuseAlbedo"] = move(diffuseAlbedo);
	//}

	//{
	//	auto fresnelR0 = make_shared<Material>();
	//	fresnelR0->SetMatCBIndex(9);
	//	fresnelR0->SetDiffuseSrvHeapIndex(GBUFFER_TEXTURE_INDEX::G_FRESNELR0);
	//	shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
	//	fresnelR0->SetShader(shader);

	//	_materials["fresnelR0"] = move(fresnelR0);
	//}

	//{
	//	auto shininess = make_shared<Material>();
	//	shininess->SetMatCBIndex(10);
	//	shininess->SetDiffuseSrvHeapIndex(GBUFFER_TEXTURE_INDEX::G_SHININESS);
	//	shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>("Forward");
	//	shininess->SetShader(shader);

	//	_materials["shininess"] = move(shininess);
	//}
}