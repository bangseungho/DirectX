#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::MatView;
Matrix Camera::MatProjection;

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

	// Regenerate Frutum
	_frustum.CreateFromMatrix(_frustum, _matProjection);
	_frustum.Transform(_frustum, _matView.Invert());
}

void Camera::Render()
{
	MatView = _matView;
	MatProjection = _matProjection;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// TODO : Layer ±¸ºÐ
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;
		
		if (gameObject->GetCheckFrustum()) {
			BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
			boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());
			if (!IsInFrustum(boundingBox))
				continue;
		}

		gameObject->GetMeshRenderer()->Render();
	}
}

bool Camera::IsInFrustum(BoundingOrientedBox& boundsOOBB)
{
	return _frustum.Intersects(boundsOOBB);
}
