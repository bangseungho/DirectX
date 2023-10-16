#include "pch.h"
#include "TestBillBoard.h"
#include "Transform.h"
#include "GameObject.h"

TestBillBoard::TestBillBoard()
{
}

TestBillBoard::~TestBillBoard()
{
}

void TestBillBoard::LateUpdate()
{
	GetTransform()->LookAt(_camera->GetTransform()->GetLocalPosition());
}
