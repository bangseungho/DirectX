#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(gEngine->GetWindow().width);
	float height = static_cast<float>(gEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	GenerateFrustum();
}

void Camera::Render()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// TODO : Layer ±¸ºÐ
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
		boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());
		
		if (gameObject->GetCheckFrustum())
			if (!IsInFrustum(boundingBox))
				continue;

		gameObject->GetMeshRenderer()->Render();
	}
}

void Camera::GenerateFrustum()
{
	_frustum.CreateFromMatrix(_frustum, _matProjection);
	_frustum.Transform(_frustum, _matView.Invert());
}

bool Camera::IsInFrustum(BoundingOrientedBox& boundsOOBB)
{
	return _frustum.Intersects(boundsOOBB);
}
