#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"
#include "GameObject.h"
#include "Collider.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()
{
	Matrix matScale = Matrix::CreateScale(mLocalScale);
	Matrix matRotation = Matrix::CreateRotationX(mLocalRotation.x);
	matRotation *= Matrix::CreateRotationY(mLocalRotation.y);
	matRotation *= Matrix::CreateRotationZ(mLocalRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(mLocalPosition);

	mMatLocal = matScale * matRotation * matTranslation;
	mMatWorld = mMatLocal;

	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		mMatWorld *= parent->GetLocalToWorldMatrix();
	}
}

void Transform::PushData()
{
	ObjectData objectData = {};
	objectData.MatWorld = mMatWorld;
	objectData.TexTransform = mTexTransform;
	objectData.MaterialIndex = GetGameObject()->GetMatIndex();
	objectData.MatViewProj = Camera::sMatView * Camera::sMatProjection;

	uint32 objCBIndex = GetGameObject()->GetObjCBIndex();
	OBJECT_CB->CopyData(objCBIndex, objectData);

	UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(objectData));

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = OBJECT_CB->Resource()->GetGPUVirtualAddress() + objCBIndex * objCBByteSize;
	CMD_LIST->SetGraphicsRootConstantBufferView(1, objCBAddress);
}

void Transform::LookAt(const Vec3& dir)
{
	Vec3 front = dir;
	front.Normalize();

	Vec3 right = Vec3::Up.Cross(dir);
	if (right == Vec3::Zero)
		right = Vec3::Forward.Cross(dir);

	right.Normalize();

	Vec3 up = front.Cross(right);
	up.Normalize();

	Matrix matrix = XMMatrixIdentity();
	matrix.Right(right);
	matrix.Up(up);
	matrix.Backward(front);

	mLocalRotation = DecomposeRotationMatrix(matrix);
}

bool Transform::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

Vec3 Transform::DecomposeRotationMatrix(const Matrix& rotation)
{
	Vec4 v[4];
	XMStoreFloat4(&v[0], rotation.Right());
	XMStoreFloat4(&v[1], rotation.Up());
	XMStoreFloat4(&v[2], rotation.Backward());
	XMStoreFloat4(&v[3], rotation.Translation());

	Vec3 ret;
	if (CloseEnough(v[0].z, -1.0f))
	{
		float x = 0;
		float y = XM_PI / 2;
		float z = x + atan2(v[1].x, v[2].x);
		ret = Vec3{ x, y, z };
	}
	else if (CloseEnough(v[0].z, 1.0f))
	{
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-v[1].x, -v[2].x);
		ret = Vec3{ x, y, z };
	}
	else
	{
		float y1 = -asin(v[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(v[1].z / cos(y1), v[2].z / cos(y1));
		float x2 = atan2f(v[1].z / cos(y2), v[2].z / cos(y2));

		float z1 = atan2f(v[0].y / cos(y1), v[0].x / cos(y1));
		float z2 = atan2f(v[0].y / cos(y2), v[0].x / cos(y2));

		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			ret = Vec3{ x1, y1, z1 };
		}
		else
		{
			ret = Vec3{ x2, y2, z2 };
		}
	}

	return ret;
}