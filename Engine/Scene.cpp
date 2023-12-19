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
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Render()
{
	// Deferred
	int8 backIndex = gEngine->GetSwapChain()->GetBackBufferIndex();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	PushCubeMapPassData();
	RenderCubeMap();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();
	mMainCamera->SortGameObject();
	mMainCamera->Render_Deferred();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	// Final
	RenderFinal();

	// Forward
	mMainCamera->Render_Forward();

	// UI
	for (auto& camera : mCameraObjects) {
		if (camera == mMainCamera)
			continue;

		if (camera == mCubeCamera)
			continue;

		camera->SortGameObject();
		camera->Render_Forward();
	}
}

void Scene::RenderCubeMap()
{
	UINT passCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));
	auto passCB = CURR_FRAMERESOURCE->mPassCB->Resource();

	mCubeCamera->SortGameObject();

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::CUBEMAP)->ClearRenderTargetView();
	for (int32 i = 0; i < RENDER_TARGET_CUBEMAP_COUNT; ++i) {
		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::CUBEMAP)->ClearDepthStencilView();

		D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress() + (i + 1) * passCBByteSize;
		GRAPHICS_CMD_LIST->SetGraphicsRootConstantBufferView(0, passCBAddress);

		gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::CUBEMAP)->OMSetRenderTargets(1, i);
		mCubeCamera->Render_Forward();
	}

	PushPassData();
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = passCB->GetGPUVirtualAddress();
	GRAPHICS_CMD_LIST->SetGraphicsRootConstantBufferView(0, passCBAddress);

	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::CUBEMAP)->WaitTargetToResource();
}

void Scene::RenderFinal()
{
	int8 backIndex = gEngine->GetSwapChain()->GetBackBufferIndex();
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	GET_SINGLE(Resources)->Get<Material>(L"Final")->Update();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}

void Scene::RenderLights()
{
	gEngine->GetMRT(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	for (auto& light : mLightObjects)
	{
		light->Render();
	}
}

void Scene::Start()
{
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->Start();
	}
}

void Scene::FixedUpdate()
{
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::Update()
{
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const sptr<GameObject>& gameObject : mGameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::AddGameObject(sptr<GameObject> gameObject)
{
	if (gameObject->GetCamera() != nullptr)
		mCameraObjects.push_back(gameObject->GetCamera());

	else if (gameObject->GetLight() != nullptr)
		mLightObjects.push_back(gameObject->GetLight());

	else if (gameObject->GetParticleSystem() != nullptr)
		mParticleObjects.push_back(gameObject);

	else if (gameObject->GetParticleSystem() != nullptr)
		mParticleObjects.push_back(gameObject);

	mGameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(sptr<GameObject> gameObject)
{
	if (gameObject->GetCamera())
	{
		auto findIt = std::find(mCameraObjects.begin(), mCameraObjects.end(), gameObject->GetCamera());
		if (findIt != mCameraObjects.end())
			mCameraObjects.erase(findIt);
	}
	else if (gameObject->GetLight())
	{
		auto findIt = std::find(mLightObjects.begin(), mLightObjects.end(), gameObject->GetLight());
		if (findIt != mLightObjects.end())
			mLightObjects.erase(findIt);
	}

	auto findIt = std::find(mGameObjects.begin(), mGameObjects.end(), gameObject);
	if (findIt != mGameObjects.end())
	{
		mGameObjects.erase(findIt);
	}
}

void Scene::PushPassData()
{
	PassConstants passConstants = {};

	passConstants.View = mMainCamera->mMatView;
	passConstants.Proj = mMainCamera->mMatProjection;
	passConstants.ViewProj = passConstants.View * passConstants.Proj;
	passConstants.EyePosW = mMainCamera->GetTransform()->GetLocalPosition();
	passConstants.CameraRight = mMainCamera->GetTransform()->GetRight();
	passConstants.NearZ = mMainCamera->mNear;
	passConstants.FarZ = mMainCamera->mFar;
	passConstants.Width = static_cast<float>(gEngine->GetWindow().Width);
	passConstants.Height = static_cast<float>(gEngine->GetWindow().Height);
	passConstants.TotalTime = TOTAL_TIME;
	passConstants.DeltaTime = DELTA_TIME;
	passConstants.AmbientLight = mAmbientLight;
	

	for (auto& light : mLightObjects) {
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(passConstants.LightCount);

		passConstants.Lights[passConstants.LightCount] = lightInfo;
		passConstants.LightCount++;
	}

	PASS_CB->CopyData(0, passConstants);
}

void Scene::PushCubeMapPassData()
{
	PassConstants passConstants = {};

	for (auto& light : mLightObjects) {
		const LightInfo& lightInfo = light->GetLightInfo();

		light->SetLightIndex(passConstants.LightCount);

		passConstants.Lights[passConstants.LightCount] = lightInfo;
		passConstants.LightCount++;
	}

	// +X -X
	for (int32 i = 0; i < 2; ++i) {
		mCubeCamera->GetTransform()->SetLocalRotation(Vec3(0.f, 90.f + i * 180.f, 0.f));
		mCubeCamera->GetTransform()->FinalUpdate();
		auto t = mCubeCamera->GetTransform()->GetLocalToWorldMatrix();

		passConstants.View = t.Invert();
		passConstants.Proj = mCubeCamera->mMatProjection;
		passConstants.ViewProj = passConstants.View * passConstants.Proj;
		passConstants.EyePosW = mCubeCamera->GetTransform()->GetLocalPosition();
		passConstants.CameraRight = mCubeCamera->GetTransform()->GetRight();
		passConstants.NearZ = mCubeCamera->mNear;
		passConstants.FarZ = mCubeCamera->mFar;
		passConstants.Width = static_cast<float>(gEngine->GetWindow().Width);
		passConstants.Height = static_cast<float>(gEngine->GetWindow().Height);
		passConstants.TotalTime = TOTAL_TIME;
		passConstants.DeltaTime = DELTA_TIME;
		passConstants.AmbientLight = mAmbientLight;

		PASS_CB->CopyData(i + 1, passConstants);
	}

	for (int32 i = 2; i < 4; ++i) {
		mCubeCamera->GetTransform()->SetLocalRotation(Vec3(-90.f + i * 180.f, 0.f, 0.f));
		mCubeCamera->GetTransform()->FinalUpdate();
		auto t = mCubeCamera->GetTransform()->GetLocalToWorldMatrix();

		passConstants.View = t.Invert();
		passConstants.Proj = mCubeCamera->mMatProjection;
		passConstants.ViewProj = passConstants.View * passConstants.Proj;
		passConstants.EyePosW = mCubeCamera->GetTransform()->GetLocalPosition();
		passConstants.CameraRight = mCubeCamera->GetTransform()->GetRight();
		passConstants.NearZ = mCubeCamera->mNear;
		passConstants.FarZ = mCubeCamera->mFar;
		passConstants.Width = static_cast<float>(gEngine->GetWindow().Width);
		passConstants.Height = static_cast<float>(gEngine->GetWindow().Height);
		passConstants.TotalTime = TOTAL_TIME;
		passConstants.DeltaTime = DELTA_TIME;
		passConstants.AmbientLight = mAmbientLight;

		PASS_CB->CopyData(i + 1, passConstants);
	}

	// +Z -Z
	for (int32 i = 4; i < 6; ++i) {
		mCubeCamera->GetTransform()->SetLocalRotation(Vec3(0.f, i * 180.f, 0.f));
		mCubeCamera->GetTransform()->FinalUpdate();
		auto t = mCubeCamera->GetTransform()->GetLocalToWorldMatrix();

		passConstants.View = t.Invert();
		passConstants.Proj = mCubeCamera->mMatProjection;
		passConstants.ViewProj = passConstants.View * passConstants.Proj;
		passConstants.EyePosW = mCubeCamera->GetTransform()->GetLocalPosition();
		passConstants.CameraRight = mCubeCamera->GetTransform()->GetRight();
		passConstants.NearZ = mCubeCamera->mNear;
		passConstants.FarZ = mCubeCamera->mFar;
		passConstants.Width = static_cast<float>(gEngine->GetWindow().Width);
		passConstants.Height = static_cast<float>(gEngine->GetWindow().Height);
		passConstants.TotalTime = TOTAL_TIME;
		passConstants.DeltaTime = DELTA_TIME;
		passConstants.AmbientLight = mAmbientLight;

		PASS_CB->CopyData(i + 1, passConstants);
	}

}

void Scene::CreateCubeMapTexture()
{
}
