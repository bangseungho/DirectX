#pragma once

class Scene;

class SceneManager
{
	SINGLETON(SceneManager)

public:
	void Update();
	void Render();
	uint32 LoadScene(wstring sceneName);
	void SetFixedTimestep(float fixedTimeStep) { _fixedTimestep = fixedTimeStep; }

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;
	float _fixedTimestep = 0.02f;
	float _currTime = 0.f;
};

