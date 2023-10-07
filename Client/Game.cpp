#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Material.h"

#include "GameObject.h"
#include "MeshRenderer.h"

sptr<GameObject> gameObject = make_shared<GameObject>();

void Game::Init(const WindowInfo& info)
{
	gEngine->Init(info);
	CMD_LIST->Reset(CMD_ALLOC.Get(), nullptr);

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

	gameObject->Init();
	
	sptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
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

	ThrowIfFailed(CMD_LIST->Close());
	ID3D12CommandList* cmdsLists[] = { CMD_LIST.Get() };
	CMD_QUEUE->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	gEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	gEngine->Update();

	gEngine->RenderBegin();

	gameObject->Update();

	gEngine->RenderEnd();
}

LRESULT Game::OnProcessingWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			GET_SINGLE(Timer)->Stop();
		else
			GET_SINGLE(Timer)->Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		//OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		//OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}
