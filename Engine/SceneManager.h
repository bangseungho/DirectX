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
	
	void SetFixedTimestep(float fixedTimeStep) { _fixedTimestep = fixedTimeStep; }

public:
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
	
private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;
	float _fixedTimestep = 0.02f;
	float _currTime = 0.f;

	array<wstring, LAYER_COUNT> _layerNames;
	map<wstring, uint8> _layerIndex;
};

