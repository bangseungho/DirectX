#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Rigidbody3D.h"
#include "Collider.h"

#include "TestCameraScript.h"
#include "TestRotationScript.h"
#include "TestAutoMoveScript.h"
#include "TestLightMoveToCamera.h"
#include "TestBillBoard.h"
#include "TestScript.h"
#include "FlashLightScript.h"

#include "Resources.h"

DECLARE_SINGLE(SceneManager)

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_currTime += DELTA_TIME;
	if (_currTime >= 0.01f) {
		_activeScene->FixedUpdate();
		_currTime = 0.f;
	}

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

uint32 SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드
	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();

	uint32 objCount = GameObject::ObjCBIndex;

	return objCount;
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	const wstring prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
//============================================================================== LayerMask
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion



//============================================================================== Scene
	shared_ptr<Scene> scene = make_shared<Scene>();
//============================================================================== Camera
#pragma region MainCamera
	{
		shared_ptr<GameObject> mainCamera = make_shared<GameObject>();
		mainCamera->Init();
		mainCamera->SetName(L"Main_Camera");
		mainCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		mainCamera->AddComponent(make_shared<TestCameraScript>());
		mainCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		mainCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		scene->SetMainCamera(mainCamera);
		scene->AddGameObject(mainCamera);
	}
#pragma endregion
#pragma region UICamera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->Init();
		camera->SetName(L"UI_Camera");
		camera->AddComponent(make_shared<Camera>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false);
		scene->AddGameObject(camera);
	}
#pragma endregion
//============================================================================== SkyBox
#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->Init();
		skybox->SetCheckFrustum(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(mesh);
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("skybox");
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion
//============================================================================== Object
#pragma region NewjeansCube
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(-150.f, 0.f, 200.f));
		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("newjeans3");
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		gameObject->AddComponent(make_shared<TestAutoMoveScript>(transform->GetLocalPosition().x));
		sptr<Rigidbody3D> rigid = make_shared<Rigidbody3D>();
		rigid->SetMass(200.f);
		gameObject->AddComponent(rigid);
		gameObject->AddComponent(make_shared<Collider>());
		scene->AddGameObject(gameObject);
	}
#pragma endregion
#pragma region WallCube
{
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(150.f, 0.f, -220.f));
	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("wall");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	sptr<Rigidbody3D> rigid = make_shared<Rigidbody3D>();
	rigid->SetMass(500.f);
	gameObject->AddComponent(rigid);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region NewjeansSphere
{
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(0.f, 0.f, 200.f));
	transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("newjeans");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	sptr<Rigidbody3D> rigid = make_shared<Rigidbody3D>();
	rigid->SetMass(100.f);
	gameObject->AddComponent(rigid);
	gameObject->AddComponent(make_shared<TestRotationScript>());
	scene->AddGameObject(gameObject);
	}
#pragma endregion
#pragma region LeatherCube
{
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(150.f, 0.f, 0.f));
	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("particle");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	sptr<Rigidbody3D> rigid = make_shared<Rigidbody3D>();
	rigid->SetMass(500.f);
	gameObject->AddComponent(rigid);
	scene->AddGameObject(gameObject);
}
#pragma endregion
//============================================================================== UI
#pragma region UIPos
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (0 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("PositionTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region UINormal
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (1 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("NormalTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region UIDiffuse
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (2 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("DiffuseTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region UIDiffuseLight
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (3 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("DiffuseLightTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region UISpecularLight
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (4 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("SpecularLightTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
#pragma region UICompute
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();
	gameObject->SetCheckFrustum(false);
	gameObject->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
	gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f + (5 * 100), 250.f, 500.f));
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		meshRenderer->SetMesh(mesh);
		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("Compute");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
//============================================================================== Light
#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetLightDirection(Vec3(1.f, -1.f, 1.f));
		light->GetLight()->SetLightStrenth(Vec3(0.5f, 0.5f, 0.5f));
		scene->AddGameObject(light);
	}
#pragma endregion
#pragma region PointLight
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetLightStrenth(Vec3(0.0f, 0.9f, 0.0f));
		light->GetLight()->SetFallOff(1.f, 200.f);

		scene->AddGameObject(light);
	}
#pragma endregion
#pragma region SpotLight
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
		light->GetLight()->SetLightStrenth(Vec3(1.0f, 0.0f, 1.0f));
		light->GetLight()->SetFallOff(1.f, 1000.f);
		light->GetLight()->SetSpotPower(30.f);
		auto flashLightScript = make_shared<FlashLightScript>();
		flashLightScript->SetCameraObject(scene->GetMainCamera());
		light->AddComponent(flashLightScript);
		
		
		scene->AddGameObject(light);
	}
#pragma endregion


	return scene;
}

