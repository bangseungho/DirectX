#include "pch.h"
#include "InputManager.h"

DECLARE_SINGLE(InputManager)

int keyList[] =
{
	VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
	'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '+',
	VK_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', VK_RETURN,
	VK_LSHIFT, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', VK_RSHIFT,
	VK_LCONTROL, VK_LMENU, VK_SPACE, VK_RMENU, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,

	VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
	VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6,
	VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3,
	VK_NUMPAD0,

	VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
};

void InputManager::Init(const WindowInfo& info)
{
	_info = info;

	for (int key : keyList) {
		mKeys[key] = KeyInfo{ KEY_STATE::NONE, false };
	}
}

void InputManager::Update()
{
	const HWND hFocusedHwnd = GetFocus();

	if (hFocusedHwnd)
	{
		for (auto& keyInfo : mKeys)
		{
			int key = keyInfo.first;
			KeyInfo& info = keyInfo.second;

			// Ű�� �����ִ�
			if (GetAsyncKeyState(key) & 0x8000)
			{
				// ������ �������� ����.
				if (info.mIsPrevPushed == false)
				{
					info.mState = KEY_STATE::TAP;
				}
				else
				{
					info.mState = KEY_STATE::PRESSED;
				}
				info.mIsPrevPushed = true;
			}
			// Ű�� �ȴ����ִ�.
			else
			{
				// ������ �����־���.
				if (info.mState == KEY_STATE::TAP || info.mState == KEY_STATE::PRESSED)
				{
					info.mState = KEY_STATE::AWAY;
				}
				else if (KEY_STATE::AWAY == info.mState)
				{
					info.mState = KEY_STATE::NONE;
				}
				info.mIsPrevPushed = false;
			}
		}

		POINT ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(_info.hwnd, &ptMouse);

		mMousePrevPos = mMousePos;
		mMousePos = XMFLOAT2(static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y));
		mMouseDir.x = mMousePos.x - mMousePrevPos.x;
		mMouseDir.y = mMousePos.y - mMousePrevPos.y;

		mMouseDir.y *= -1.f;
	}

	// �����찡 ��Ŀ�� �Ǿ����� �ʴ�.
	else
	{
		for (auto& keyInfo : mKeys)
		{
			KeyInfo& info = keyInfo.second;

			info.mIsPrevPushed = false;
			if (KEY_STATE::TAP == info.mState || KEY_STATE::PRESSED == info.mState)
			{
				info.mState = KEY_STATE::AWAY;
			}
			else if (KEY_STATE::AWAY == info.mState)
			{
				info.mState = KEY_STATE::NONE;
			}
		}
	}
}