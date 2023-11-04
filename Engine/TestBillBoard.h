#pragma once
#include "MonoBehaviour.h"

class TestBillBoard : public MonoBehaviour
{
public:
	TestBillBoard();
	virtual ~TestBillBoard();

	virtual void LateUpdate() override;

public:
	void SetCamera(sptr<class GameObject> cam) { mCamera = cam; }

private:
	sptr<class GameObject> mCamera;
};

