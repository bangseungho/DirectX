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

#include "TestCameraScript.h"
#include "TestRotationScript.h"
#include "TestAutoMoveScript.h"
#include "TestLightMoveToCamera.h"
#include "TestBillBoard.h"

#include "Resources.h"

DECLARE_SINGLE(SceneManager)

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	if (KEY_PRESSED('1'))
		_activeScene->SetMainCamera(FIRST_CAMERA);
	if (KEY_PRESSED('2'))
		_activeScene->SetMainCamera(SECOND_CAMERA);

	_currTime += DELTA_TIME;
	if (_currTime >= 0.02f) {
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

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera1
	shared_ptr<GameObject> camera = make_shared<GameObject>();
	camera->Init();
	camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
	camera->AddComponent(make_shared<TestCameraScript>());
	camera->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
	scene->AddCameraObject(FIRST_CAMERA, camera);

	scene->SetMainCamera(FIRST_CAMERA);
#pragma endregion

#pragma region Camera2
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->Init();
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(100.f, 100.f, 0.f));
		scene->AddCameraObject(SECOND_CAMERA, camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->Init();

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Sky.hlsl", { RASTERIGER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
			texture->Init(L"..\\Resources\\Texture\\Sky.dds", TEXTURE_TYPE::TEXTURECUBE);
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(3, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion


#pragma region Grid
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(0.f, 0.f, 0.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadGridMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
			texture->Init(L"..\\Resources\\Texture\\grid.dds");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetMatTransform(Matrix::CreateScale(100.f));
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		scene->AddGameObject(gameObject);
	}
#pragma endregion

#pragma region Cube
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(-150.f, 50.f, 200.f));
		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
			texture->Init(L"..\\Resources\\Texture\\newjeans3.dds");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		gameObject->AddComponent(make_shared<TestAutoMoveScript>(transform->GetLocalPosition().x));

		scene->AddGameObject(gameObject);
	}
#pragma endregion

#pragma region NormalCube
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(150.f, 100.f, -220.f));
		transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shared_ptr<Texture> textureNormal = make_shared<Texture>();
			shared_ptr<Texture> textureRoughness = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
			texture->Init(L"..\\Resources\\Texture\\Leather.dds");
			textureNormal->Init(L"..\\Resources\\Texture\\Leather_Normal_BC7.dds");
			textureRoughness->Init(L"..\\Resources\\Texture\\Leather_Weave_004_roughness.dds");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetFresnel(Vec3(0.8f, 0.8f, 0.8f));
			material->SetTexture(0, texture);
			material->SetTexture(1, textureNormal);
			material->SetTexture(2, textureRoughness);
			material->SetNormalMapping(NORMAL_MAPPING_ON);
			meshRenderer->SetMaterial(material);
		}
		//sptr<TestBillBoard> testBillBoard = make_shared<TestBillBoard>();
		//testBillBoard->SetCamera(camera);
		//gameObject->AddComponent(testBillBoard);
		


		gameObject->AddComponent(meshRenderer);
		scene->AddGameObject(gameObject);
	}
#pragma endregion

#pragma region NormalCube
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(150.f, 100.f, 0.f));
		transform->SetLocalScale(Vec3(200.f, 200.f, 200.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shared_ptr<Texture> textureNormal = make_shared<Texture>();
			shared_ptr<Texture> textureRoughness = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
			texture->Init(L"..\\Resources\\Texture\\Sci-Fi_Wall_014_basecolor.dds");
			textureNormal->Init(L"..\\Resources\\Texture\\Sci-Fi_Wall_014_normal_BC7.dds");
			textureRoughness->Init(L"..\\Resources\\Texture\\Sci-Fi_Wall_014_roughness.dds");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetFresnel(Vec3(0.9f, 0.9f, 0.9f));
			material->SetTexture(0, texture);
			material->SetTexture(1, textureNormal);
			material->SetTexture(2, textureRoughness);
			material->SetNormalMapping(NORMAL_MAPPING_ON);
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		scene->AddGameObject(gameObject);
	}
#pragma endregion

#pragma region Sphere
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(0.f, 50.f, 200.f));
		transform->SetLocalScale(Vec3(100.f, 100.f, 100.f));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shader->Init(L"..\\Resources\\Shader\\Default.hlsl");
			texture->Init(L"..\\Resources\\Texture\\newjeans.dds");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
			material->SetRoughness(0.01f);
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		gameObject->AddComponent(make_shared<Rigidbody3D>());
		gameObject->AddComponent(make_shared<TestRotationScript>());
		scene->AddGameObject(gameObject);
	}
#pragma endregion

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
		light->GetLight()->SetLightStrenth(Vec3(0.9f, 0.9f, 0.9f));
		light->GetTransform()->SetLocalPosition(Vec3(-200.f, 100.f, 100.f));
		light->GetLight()->SetFallOff(1.f, 1000.f);

		sptr<TestLightMoveToCamera> moveTo = make_shared<TestLightMoveToCamera>();
		moveTo->SetGameObject(camera);
		light->AddComponent(moveTo);

		scene->AddGameObject(light);
	}
#pragma endregion
	//
	//#pragma region SpotLight
	//	{
	//		shared_ptr<GameObject> light = make_shared<GameObject>();
	//		light->AddComponent(make_shared<Transform>());
	//		light->AddComponent(make_shared<Light>());
	//		light->GetTransform()->SetLocalPosition(Vec3(-50.f, 200.f, 200.f));
	//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
	//		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
	//		light->GetLight()->SetLightStrenth(Vec3(1.0f, 0.0f, 1.0f));
	//		light->GetLight()->SetFallOff(1.f, 500.f);
	//
	//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	//		{
	//			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectMesh();
	//			meshRenderer->SetMesh(mesh);
	//		}
	//		{
	//			shared_ptr<Shader> shader = make_shared<Shader>();
	//			shared_ptr<Texture> texture = make_shared<Texture>();
	//			shader->Init(L"..\\Resources\\Shader\\GUI.hlsl");
	//			texture->Init(L"..\\Resources\\Texture\\light.dds");
	//			shared_ptr<Material> material = make_shared<Material>();
	//			material->SetFresnel(Vec3(0.1f, 0.1f, 0.1f));
	//			material->SetRoughness(0.01f);
	//			material->SetShader(shader);
	//			material->SetTexture(0, texture);
	//			meshRenderer->SetMaterial(material);
	//		}
	//		light->AddComponent(meshRenderer);
	//		scene->AddGameObject(light);
	//	}
	//#pragma endregion


	return scene;
}