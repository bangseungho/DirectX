#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include "TestCameraScript.h"
#include "TestRotationScript.h"
#include "TestAutoMoveScript.h"

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

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_activeScene == nullptr)
		return;

	const shared_ptr<GameObject>& cameraObjects = _activeScene->GetMainCamera();
	
	cameraObjects->GetCamera()->Render();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : ���� Scene ����
	// TODO : ���Ͽ��� Scene ���� �ε�

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera1
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->Init();
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
		scene->AddCameraObject(FIRST_CAMERA, camera);
	}

	scene->SetMainCamera(FIRST_CAMERA);
#pragma endregion

#pragma region Camera2
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->Init();
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
		camera->AddComponent(make_shared<TestCameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(100.f, 100.f, 0.f));
		scene->AddCameraObject(SECOND_CAMERA, camera);
	}
#pragma endregion

#pragma region Cube
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(-100.f, 100.f, 200.f));
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
		gameObject->AddComponent(make_shared<TestAutoMoveScript>(gameObject->GetTransform()->GetLocalPosition().x));
		scene->AddGameObject(gameObject);
	}
#pragma endregion

#pragma region Sphere
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->Init();

		shared_ptr<Transform> transform = gameObject->GetTransform();
		transform->SetLocalPosition(Vec3(70.f, 100.f, 200.f));
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
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		gameObject->AddComponent(meshRenderer);
		gameObject->AddComponent(make_shared<TestRotationScript>());
		scene->AddGameObject(gameObject);
	}
#pragma endregion

	return scene;
}