#include "pch.h"
#include "Scene.h"
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
}

void Scene::Render()
{
	PushPassData();

	int8 backIndex = gEngine->GetSwapChain()->GetBackBufferIndex();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	_mainCamera->SortGameObject();
	_mainCamera->Render_Deferred();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();
	RenderFinal();

	_mainCamera->Render_Forward();

	for (auto& camera : _cameraObjects) {
		if (camera == _mainCamera)
			continue;

		camera->SortGameObject();
		camera->Render_Forward();
	}
}

void Scene::RenderFinal()
{
	int8 backIndex = gEngine->GetSwapChain()->GetBackBufferIndex();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	GET_SINGLE(Resources)->Get<Material>("Final")->Update();
	GET_SINGLE(Resources)->Get<Mesh>("Rectangle")->Render();
}

void Scene::RenderLights()
{
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	for (auto& light : _lightObjects)
	{
		light->Render();
	}
}

void Scene::Start()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::FixedUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::Update()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const sptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::AddGameObject(sptr<GameObject> gameObject)
{
	if (gameObject->GetCamera() != nullptr)
		_cameraObjects.push_back(gameObject->GetCamera());

	else if (gameObject->GetLight() != nullptr)
		_lightObjects.push_back(gameObject->GetLight());

	_gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(sptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(_cameraObjects.begin(), _cameraObjects.end(), gameObject->GetCamera());
		if (findIt != _cameraObjects.end())
			_cameraObjects.erase(findIt);
	}
	else if (gameObject->GetLight())
	{
		auto findIt = std::find(_lightObjects.begin(), _lightObjects.end(), gameObject->GetLight());
		if (findIt != _lightObjects.end())
			_lightObjects.erase(findIt);
	}

	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}

void Scene::PushPassData()
{
	PassConstants passConstants = {};

	passConstants.view = _mainCamera->_matView;
	passConstants.proj = _mainCamera->_matProjection;
	passConstants.viewProj = passConstants.view * passConstants.proj;
	passConstants.eyePosW = _mainCamera->GetTransform()->GetLocalPosition();
	passConstants.nearZ = _mainCamera->_near;
	passConstants.farZ = _mainCamera->_far;
	passConstants.width = gEngine->GetWindow().width;
	passConstants.height = gEngine->GetWindow().height;
	passConstants.totalTime = TOTAL_TIME;
	passConstants.deltaTime = DELTA_TIME;
	passConstants.ambientLight = _ambientLight;

	for (auto& light : _lightObjects) {
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(passConstants.lightCount);

		passConstants.lights[passConstants.lightCount] = lightInfo;
		passConstants.lightCount++;
	}

	PASS_CB->CopyData(0, passConstants);
}