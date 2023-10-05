#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
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

	mesh->Init(vec, indexVec);

	shader->Init(L"..\\Resources\\Shader\\Default.hlsl");

	texture->Init(L"..\\Resources\\Texture\\newjeans3.dds");

	ThrowIfFailed(CMD_LIST->Close());
	ID3D12CommandList* cmdsLists[] = { CMD_LIST.Get() };
	CMD_QUEUE->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->Update();

	GEngine->RenderBegin();

	shader->Update();

	{
		ObjectConstants o;
		o.offset = Vec4(0.0f, 0.f, 0.f, 0.f);
		o.color = Vec4(0.0f, 0.f, 0.f, 0.f);
		mesh->SetObjectConstant(o);

		mesh->SetTexture(texture);

		mesh->Render();
	}

	GEngine->RenderEnd();
}
