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
#include "Terrain.h"

#include "TestCameraScript.h"
#include "TestRotationScript.h"
#include "TestAutoMoveScript.h"
#include "TestScript.h"
#include "FlashLightScript.h"
#include "GunPlayerScript.h"
#include "BulletScript.h"
#include "ScoreScript.h"

#include "Resources.h"

default_random_engine dre;

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
	sptr<GameObject> sceneMainCamera;
//============================================================================== Camera
#pragma region MainCamera
	{
		sptr<GameObject> mainCamera = make_shared<GameObject>();
		mainCamera->Init();
		mainCamera->SetName(L"Main_Camera");
		mainCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		mainCamera->AddComponent(make_shared<TestCameraScript>());
		mainCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		mainCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true);
		scene->SetMainCamera(mainCamera);
		scene->AddGameObject(mainCamera);
		sceneMainCamera = mainCamera;
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
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"skybox");
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion
//============================================================================== Object
//#pragma region NewjeansCube
//	{
//		sptr<GameObject> gameObject = make_shared<GameObject>();
//		gameObject->Init();
//
//		sptr<Transform> transform = gameObject->GetTransform();
//		transform->SetLocalPosition(Vec3(-150.f, 0.f, 200.f));
//		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//
//		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(mesh);
//			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"newjeans3");
//			meshRenderer->SetMaterial(material);
//		}
//		gameObject->AddComponent(make_shared<Collider>());
//		gameObject->AddComponent(meshRenderer);
//		gameObject->AddComponent(make_shared<TestAutoMoveScript>(transform->GetLocalPosition().x));
//		gameObject->AddComponent(make_shared<Collider>());
//		scene->AddGameObject(gameObject);
//	}
//#pragma endregion
//#pragma region WallCube
//{
//	sptr<GameObject> gameObject = make_shared<GameObject>();
//	gameObject->Init();
//
//	sptr<Transform> transform = gameObject->GetTransform();
//	transform->SetLocalPosition(Vec3(150.f, 0.f, -220.f));
//	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));
//
//	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//	{
//		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
//		meshRenderer->SetMesh(mesh);
//		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"wall");
//		meshRenderer->SetMaterial(material);
//	}
//	gameObject->AddComponent(make_shared<Collider>());
//	gameObject->AddComponent(meshRenderer);
//	scene->AddGameObject(gameObject);
//}
//#pragma endregion
//#pragma region NewjeansSphere
//uniform_int_distribution uid{ -3000, 3000 };
//uniform_int_distribution uidMass{ 1, 300 };
//uniform_int_distribution uidMat{ 0, 2 };
//
//vector<wstring> mats = {
//		L"newjeans",
//		L"newjeans2",
//		L"newjeans3"
//};
//
//for (int i = 0; i < 100; ++i) {
//		sptr<GameObject> gameObject = make_shared<GameObject>();
//		gameObject->Init();
//
//		sptr<Transform> transform = gameObject->GetTransform();
//		transform->SetLocalPosition(Vec3(uid(dre), 100.f, uid(dre)));
//		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//
//		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
//			meshRenderer->SetMesh(mesh);
//			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(mats[uidMat(dre)]);
//			meshRenderer->SetMaterial(material);
//		}
//		gameObject->AddComponent(make_shared<Collider>());
//		gameObject->AddComponent(meshRenderer);
//		sptr<Rigidbody3D> rigid = make_shared<Rigidbody3D>();
//		rigid->SetMass(uidMass(dre));
//		gameObject->AddComponent(rigid);
//		gameObject->AddComponent(make_shared<TestRotationScript>());
//		scene->AddGameObject(gameObject);
//	}
//#pragma endregion
//#pragma region LeatherCube
//{
//	sptr<GameObject> gameObject = make_shared<GameObject>();
//	gameObject->Init();
//
//	sptr<Transform> transform = gameObject->GetTransform();
//	transform->SetLocalPosition(Vec3(150.f, 0.f, 0.f));
//	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));
//
//	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//	{
//		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
//		meshRenderer->SetMesh(mesh);
//		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"leather");
//		meshRenderer->SetMaterial(material);
//	}
//	gameObject->AddComponent(meshRenderer);
//	gameObject->AddComponent(make_shared<Collider>());
//	scene->AddGameObject(gameObject);
//}
//#pragma endregion

#pragma endregion
#pragma region Sphere
{
	sptr<GameObject> gameObject = make_shared<GameObject>();
	gameObject->Init();

	sptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
	transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		meshRenderer->SetMesh(mesh);
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"mirror");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"PositionTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"NormalTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"DiffuseTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"DiffuseLightTarget");
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
		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"SpecularLightTarget");
		meshRenderer->SetMaterial(material);
	}
	gameObject->AddComponent(meshRenderer);
	scene->AddGameObject(gameObject);
}
#pragma endregion
//#pragma region UIScore
//{
//	sptr<GameObject> gameObject = make_shared<GameObject>();
//	gameObject->Init();
//	gameObject->SetCheckFrustum(false);
//
//	array<sptr<GameObject>, 3> scores;
//	for (int i = 0; i < 3; ++i) {
//		scores[i] = make_shared<GameObject>();
//		scores[i]->Init();
//		scores[i]->SetCheckFrustum(false);
//		scores[i]->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI"));
//		scores[i]->GetTransform()->SetLocalScale(Vec3(70.f, 70.f, 70.f));
//		scores[i]->GetTransform()->SetLocalPosition(Vec3(320 - i * 70, 250.f, 500.f));
//
//		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
//			meshRenderer->SetMesh(mesh);
//			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Number0");
//			meshRenderer->SetMaterial(material);
//		}
//		scores[i]->AddComponent(meshRenderer);
//		scene->AddGameObject(scores[i]);
//	}
//	auto scoreScript = make_shared<ScoreScript>();
//	scoreScript->SetScores(scores);
//	GET_SINGLE(Resources)->Add<Component>(L"ScoreScript", scoreScript);
//	gameObject->AddComponent(scoreScript);
//
//	scene->AddGameObject(gameObject);
//}
//#pragma endregion
//============================================================================== Light
#pragma region Directional Light
	{
		sptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetLightDirection(Vec3(0.57735f, -0.57735f, 0.57735f));
		light->GetLight()->SetLightStrenth(Vec3(0.6f, 0.6f, 0.6f));
		scene->AddGameObject(light);
	}
#pragma endregion
//#pragma region PointLight
//	{
//		sptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetLightStrenth(Vec3(0.0f, 0.9f, 0.0f));
//		light->GetLight()->SetFallOff(1.f, 200.f);
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//#pragma region SpotLight
//	{
//		sptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->AddComponent(make_shared<Light>());
//		light->GetTransform()->SetLocalPosition(Vec3(328.f, 345.f, 389.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
//		light->GetLight()->SetLightStrenth(Vec3(0.f, 100.f, 100.f));
//		light->GetLight()->SetFallOff(1.f, 1000.f);
//		light->GetLight()->SetSpotPower(100.f);
//		scene->AddGameObject(light);
//	}
//#pragma endregion

//#pragma region SpotLight
//	{
//		sptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->AddComponent(make_shared<Light>());
//		light->GetTransform()->SetLocalPosition(Vec3(379.f, 345.f, 327.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
//		light->GetLight()->SetLightStrenth(Vec3(20.f, 50.f, 100.f));
//		light->GetLight()->SetFallOff(1.f, 1000.f);
//		light->GetLight()->SetSpotPower(100.f);
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//#pragma region SpotLight
//	{
//		sptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->AddComponent(make_shared<Light>());
//		light->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
//		light->GetLight()->SetLightStrenth(Vec3(2.0f, 1.0f, 0.4f));
//		light->GetLight()->SetFallOff(1.f, 1000.f);
//		light->GetLight()->SetSpotPower(30.f);
//
//		auto flashLightScript = make_shared<FlashLightScript>();
//		flashLightScript->SetCameraObject(sceneMainCamera);
//		light->AddComponent(flashLightScript);
//		
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//============================================================================== ParticleSystem
//#pragma region ParticleSystem
//	{
//		sptr<GameObject> particle = make_shared<GameObject>();
//		particle->AddComponent(make_shared<Transform>());
//		particle->SetCheckFrustum(false);
//		particle->GetTransform()->SetParent(sceneMainCamera->GetTransform());
//
//		auto particleSystem = make_shared<ParticleSystem>();
//		particleSystem->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"SnowParticle"));
//		particleSystem->SetComputeMaterial(GET_SINGLE(Resources)->Get<Material>(L"Compute_Snow_Particle"));
//		particleSystem->SetParticleIndex(0);
//
//		auto& particleSystemData = particleSystem->GetParticleSystemData();
//		particleSystemData.MaxCount = 300;
//		particleSystemData.MinSpeed = 10;
//		particleSystemData.MaxSpeed = 1;
//		particleSystemData.StartScale = 1.f;
//		particleSystemData.EndScale = 0.5f;
//
//		particle->AddComponent(particleSystem);
//		scene->AddGameObject(particle);
//	}
//#pragma endregion
//#pragma region ParticleSystem
//	{
//		for (int i = 0; i < 50; ++i) {
//			sptr<GameObject> particle = make_shared<GameObject>();
//			particle->AddComponent(make_shared<Transform>());
//
//			sptr<ParticleSystem> particleSystem = make_shared<HitParticleSystem>();
//			particleSystem->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"lightParticle"));
//			particleSystem->SetParticleIndex(1);
//			particle->AddComponent(particleSystem);
//			particle->SetActive(false);
//
//			particle->SetCheckFrustum(false);
//			particle->GetTransform()->SetLocalPosition(Vec3(-50.f, 100.f, 100.f));
//			particle->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//			scene->AddGameObject(particle);
//		}
//	}
//#pragma endregion
//============================================================================== Terrain
#pragma region Terrain
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<Terrain>());
		obj->AddComponent(make_shared<MeshRenderer>());

		sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadTerrainMesh(64, 64);
			meshRenderer->SetMesh(mesh);
			sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);

		obj->GetTransform()->SetLocalScale(Vec3(100.f, 400.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-1000.f, -300.f, -1000.f));
		obj->GetTerrain()->Init(64, 64);
		obj->SetCheckFrustum(false);

		scene->AddGameObject(obj);
	}
#pragma endregion

//#pragma region FBX
//	{
//		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Building.fbx");
//
//		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
//
//		for (auto& gameObject : gameObjects)
//		{
//			gameObject->SetName(L"Building");
//			gameObject->SetCheckFrustum(false);
//			gameObject->GetTransform()->SetLocalPosition(Vec3(500.f, -100.f, 500.f));
//			gameObject->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//			scene->AddGameObject(gameObject);
//		}
//	}
//#pragma endregion

//#pragma region FBX
//	{
//		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Dragon.fbx");
//
//		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
//
//		for (auto& gameObject : gameObjects)
//		{
//			gameObject->SetName(L"Dragon");
//			gameObject->SetCheckFrustum(false);
//			gameObject->GetTransform()->SetLocalPosition(Vec3(1000.f, -100.f, 500.f));
//			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 90.f, 0.f));
//			gameObject->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
//			scene->AddGameObject(gameObject);
//		}
//	}
//#pragma endregion

#pragma region GunPlayer
	{
		shared_ptr<GameObject> gunObject = make_shared<GameObject>();
		gunObject->Init();

		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Sniper.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		auto gunPlayerScript = make_shared<GunPlayerScript>();
		gunPlayerScript->SetCameraObject(sceneMainCamera);

		scene->AddGameObject(gunObject);
		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Sniper");
			gameObject->SetCheckFrustum(false);
			gunPlayerScript->GetGunParts().push_back(gameObject);
			scene->AddGameObject(gameObject);
		}
		
		//for (int i = 0; i < 50; ++i) {
		//	shared_ptr<GameObject> bullet = make_shared<GameObject>();
		//	bullet->Init();
		//	bullet->SetActive(false);
		//	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		//	{
		//		sptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		//		meshRenderer->SetMesh(mesh);
		//		sptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"newjeans");
		//		meshRenderer->SetMaterial(material);
		//	}
		//	bullet->GetTransform()->SetLocalScale(Vec3(5.f));
		//	bullet->GetTransform()->SetLocalPosition(Vec3(8.f, 0.7f, 0.f));
		//	auto bulletScript = make_shared<BulletScript>();
		//	bullet->AddComponent(bulletScript);
		//	bullet->AddComponent(meshRenderer);
		//	scene->AddGameObject(bullet);

		//	gunPlayerScript->GetBullets().push_back(bullet);
		//}
		
		gunObject->AddComponent(gunPlayerScript);
	}
#pragma endregion
	return scene;
}

