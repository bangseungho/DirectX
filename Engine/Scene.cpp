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

	_mainCamera->SortGameObject();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	_mainCamera->Render_Deferred();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);
	_mainCamera->Render_Forward();

	for (auto& camera : _cameraObjects) {
		if (camera == _mainCamera)
			continue;

		camera->SortGameObject();
		camera->Render_Forward();
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