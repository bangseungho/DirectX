#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Material.h"
#include "SceneManager.h"
#include "Resources.h"

void Game::Init(const WindowInfo& info)
{
	gEngine->Init(info);

	uint32 objectCount = GET_SINGLE(SceneManager)->LoadScene(L"TestScene");
	gEngine->BuildFrameResource(DEVICE, objectCount);

	ThrowIfFailed(CMD_LIST->Close());
	ID3D12CommandList* cmdsLists[] = { CMD_LIST.Get() };
	CMD_QUEUE->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	gEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	gEngine->Update();
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
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		break;
	}
	return(0);
}
