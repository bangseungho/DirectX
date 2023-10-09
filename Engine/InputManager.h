#pragma once
enum class KEY_STATE 
{	
	NONE, 
	TAP, 
	PRESSED, 
	AWAY
};

struct KeyInfo {
	KEY_STATE mState{};
	bool mIsPrevPushed{};
};

class InputManager {
	SINGLETON(InputManager)

public:
	void Init(const WindowInfo& info);
	void Update();

public:
	KEY_STATE GetKeyState(int key)
	{
		return mKeys[key].mState;
	}

	Vec2 GetMousePos() const { return mMousePos; }
	Vec2 GetMouseDir() const { return mMouseDir; }

	Vec2 GetMouseDelta() const
	{
		Vec2 vDelta = Vec2(mMousePos.x - mMousePrevPos.x, mMousePos.y - mMousePrevPos.y);
		return vDelta;
	}

private:
	WindowInfo _info;
	std::unordered_map<int, KeyInfo> mKeys;
	Vec2  mMousePos;
	Vec2  mMousePrevPos;
	Vec2  mMouseDir;
};


#define KEY_NONE(key)		GET_SINGLE(InputManager)->GetKeyState(key) == KEY_STATE::NONE
#define KEY_TAP(key)		GET_SINGLE(InputManager)->GetKeyState(key) == KEY_STATE::TAP
#define KEY_PRESSED(key)	GET_SINGLE(InputManager)->GetKeyState(key) == KEY_STATE::PRESSED
#define KEY_AWAY(key)		GET_SINGLE(InputManager)->GetKeyState(key) == KEY_STATE::AWAY