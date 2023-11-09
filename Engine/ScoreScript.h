#pragma once
#include "MonoBehaviour.h"

class ScoreScript : public MonoBehaviour
{
public:
	ScoreScript() {};
	virtual ~ScoreScript() {};

	void SetScores(array<sptr<GameObject>, 3>& scores) { mScores = scores; }

	void AddScore() { mScoreCount++; }

	virtual void LateUpdate() override;

private:
	array<sptr<GameObject>, 3> mScores;
	uint32 mScoreCount = 0;
};

