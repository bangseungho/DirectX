#include "pch.h"
#include "ScoreScript.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Resources.h"

void ScoreScript::LateUpdate()
{
	uint32 scoreCount = mScoreCount;
	wstring wsScoreCount[3];
	for (int i = 0; i < 3; ++i) {
		wstring remainder = to_wstring(scoreCount % 10);
		scoreCount /= 10;
		wsScoreCount[i] = L"Number" + remainder;
	}
	
	for (int i = 0; i < 3; ++i) {
		auto meshRenderer = mScores[i]->GetMeshRenderer();
		auto material = GET_SINGLE(Resources)->Get<Material>(wsScoreCount[i]);
		meshRenderer->SetMaterial(material);
		mScores[i]->SetMatIndex(material->GetMatCBIndex());
	}
}
