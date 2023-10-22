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

//void Camera::Render()
//{
//	MatView = _matView;
//	MatProjection = _matProjection;
//
//	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
//
//	// TODO : Layer ±¸ºÐ
//	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
//
//	for (auto& gameObject : gameObjects)
//	{
//		if (gameObject->GetMeshRenderer() == nullptr)
//			continue;
//
//		if (IsCulled(gameObject->GetLayerIndex()))
//			continue;
//		
//		if (gameObject->GetCheckFrustum()) {
//			BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
//			boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());
//			if (!IsInFrustum(boundingBox))
//				continue;
//		}
//
//		gameObject->GetMeshRenderer()->Render();
//	}
//}

bool Camera::IsInFrustum(BoundingOrientedBox& boundsOOBB)
{
	return _frustum.Intersects(boundsOOBB);
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();

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

		SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
		switch (shaderType)
		{
		case SHADER_TYPE::DEFERRED:
			_vecDeferred.push_back(gameObject);
			break;
		case SHADER_TYPE::FORWARD:
			_vecForward.push_back(gameObject);
			break;
		}
	}
}

void Camera::Render_Deferred()
{
	MatView = _matView;
	MatProjection = _matProjection;

	for (auto& gameObject : _vecDeferred)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}

void Camera::Render_Forward()
{
	MatView = _matView;
	MatProjection = _matProjection;

	for (auto& gameObject : _vecForward)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}