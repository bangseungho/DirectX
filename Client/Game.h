#pragma once

class Game
{
public:
	~Game();

public:
	void Init(const WindowInfo& info);
	void Update();

	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

