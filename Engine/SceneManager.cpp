#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"

DECLARE_SINGLE(SceneManager)

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
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

	// TestObject
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.f, 0.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.f, 0.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.f, 1.f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.f, 1.f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	gameObject->Init(); // Transform

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}

	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\Default.hlsl");

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Init(L"..\\Resources\\Texture\\newjeans3.dds");

		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetDiffuse(Vec4(0.5f, 0.5f, 0.5f, 1.f));
		material->SetFresnel(Vec3(0.01f, 0.01f, 0.01f));
		material->SetRoughness(0.5f);
		material->SetTexOn(1.f);
		material->SetTexture(0, texture);
		meshRenderer->SetMaterial(material);
	}

	gameObject->AddComponent(meshRenderer);

	scene->AddGameObject(gameObject);

	return scene;
}