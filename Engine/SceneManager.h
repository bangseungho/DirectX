#pragma once

class Scene;

enum
{
	LAYER_COUNT = 32,
};

class SceneManager
{
	SINGLETON(SceneManager)

public:
	void Update();
	void Render();
	
	uint32 LoadScene(wstring sceneName);
	
	void SetFixedTimestep(float fixedTimeStep) { mFixedTimeStep = fixedTimeStep; }

public:
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return mLayerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return mActiveScene; }
	
private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene>				mActiveScene;
	float							mFixedTimeStep = 0.02f;
	float							mCurrTime = 0.f;

	array<wstring, LAYER_COUNT>		mLayerNames;
	map<wstring, uint8>				mLayerIndex;
};

