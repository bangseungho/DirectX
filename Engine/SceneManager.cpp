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
#include "ParticleSystem.h"

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
	if (mActiveScene == nullptr)
		return;

	mCurrTime += DELTA_TIME;
	if (mCurrTime >= 0.01f) {
		mActiveScene->FixedUpdate();
		mCurrTime = 0.f;
	}

	mActiveScene->Update();
	mActiveScene->LateUpdate();
	mActiveScene->FinalUpdate();
}

void SceneManager::Render()
{
	//shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>("Compute");
	//material->Dispatch(1, 1024, 1);

	if (mActiveScene)
		mActiveScene->Render();
}

uint32 SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드
	mActiveScene = LoadTestScene();

	mActiveScene->Awake();
	mActiveScene->Start();

	uint32 objCount = GameObject::ObjCBIndex;

	return objCount;
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	const wstring prevName = mLayerNames[index];
	mLayerIndex.erase(prevName);

	mLayerNames[index] = name;
	mLayerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = mLayerIndex.find(name);
	if (findIt == mLayerIndex.end())
		return 0;

	return findIt->second;
}

sptr<Scene> SceneManager::LoadTestScene()
{
//============================================================================== LayerMask
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

//============================================================================== Scene
	sptr<Scene> scene = make_shared<Scene>();
//============================================================================== Camera
#pragma region MainCamera
	{
		sptr<GameObject> mainCamera = make_shared<GameObject>();
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
		sptr<GameObject> camera = make_shared<GameObject>();
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
		sptr<GameObject> skybox = make_shared<GameObject>();
		skybox->Init();
		skybox->SetCheckFrustum(false);

		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(mesh);
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("skybox");
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion
//============================================================================== Object
#pragma region NewjeansCube
	{
		sptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		sptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(-150.f, 0.f, 200.f));
		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("newjeans3");
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
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	sptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(150.f, 0.f, -220.f));
	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(mesh);
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("wall");
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
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	sptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(0.f, 0.f, 200.f));
	transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		meshRenderer->SetMesh(mesh);
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("newjeans");
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
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	sptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(150.f, 0.f, 0.f));
	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(mesh);
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("leather");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("PositionTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("NormalTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("DiffuseTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("DiffuseLightTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("SpecularLightTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
//============================================================================== Light
#pragma region Directional Light
	{
		sptr<GameObject> light = make_shared<GameObject>();
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
		sptr<GameObject> light = make_shared<GameObject>();
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
		sptr<GameObject> light = make_shared<GameObject>();
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
//============================================================================== ParticleSystem

#pragma region ParticleSystem
	{
		sptr<GameObject> particle = make_shared<GameObject>();
		particle->AddComponent(make_shared<Transform>());

		auto particleSystem = make_shared<ParticleSystem>();
		particleSystem->SetMaterial(GET_SINGLE(Resources)->Get<Material>("fireParticle"));
		particle->AddComponent(particleSystem);

		particle->SetCheckFrustum(false);
		particle->GetTransform()->SetLocalPosition(Vec3(50.f, 100.f, 100.f));
		particle->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		scene->AddGameObject(particle);
	}
#pragma endregion

#pragma region ParticleSystem
	{
		sptr<GameObject> particle = make_shared<GameObject>();
		particle->AddComponent(make_shared<Transform>());

		auto particleSystem = make_shared<ParticleSystem>();
		particleSystem->SetMaterial(GET_SINGLE(Resources)->Get<Material>("lightParticle"));
		particle->AddComponent(particleSystem);

		particle->SetCheckFrustum(false);
		particle->GetTransform()->SetLocalPosition(Vec3(-50.f, 100.f, 100.f));
		particle->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		scene->AddGameObject(particle);
	}
#pragma endregion

#pragma region Tessellation
	{
		sptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->GetTransform()->SetLocalPosition(Vec3(0, 0, 300.f));
		gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));

		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>("Tessellation");
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		scene->AddGameObject(gameObject);
	}
#pragma endregion
	return scene;
}

