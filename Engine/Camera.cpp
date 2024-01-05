#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ParticleSystem.h"
#include "Engine.h"

Matrix Camera::sMatView;
Matrix Camera::sMatProjection;
Matrix Camera::sMatShadowViewProj;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	mWidth = static_cast<float>(gEngine->GetWindow().Width);
	mHeight = static_cast<float>(gEngine->GetWindow().Height);
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	mMatView = GetTransform()->GetLocalToWorldMatrix().Invert();

	if (mProjectionType == PROJECTION_TYPE::PERSPECTIVE)
		mMatProjection = ::XMMatrixPerspectiveFovLH(mFov, mWidth / mHeight, mNear, mFar);
	else
		mMatProjection = ::XMMatrixOrthographicLH(mWidth * mScale, mHeight * mScale, mNear, mFar);

	// Regenerate Frutum
	mFrustum.CreateFromMatrix(mFrustum, mMatProjection);
	mFrustum.Transform(mFrustum, mMatView.Invert());
}

bool Camera::IsInFrustum(BoundingOrientedBox& boundsOOBB)
{
	return mFrustum.Intersects(boundsOOBB);
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	mForwardObjects.clear();
	mDeferredObjects.clear();
	mParticleObjects.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr && gameObject->GetParticleSystem() == nullptr)
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum()) {
			BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
			boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());
			if (!IsInFrustum(boundingBox))
				continue;
		}

		if (gameObject->GetMeshRenderer()) {
			SHADER_TYPE ShaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (ShaderType)
			{
			case SHADER_TYPE::DEFERRED:
				mDeferredObjects.push_back(gameObject);
				break;
			case SHADER_TYPE::FORWARD:
				mForwardObjects.push_back(gameObject);
				break;
			}
		}
		else {
			mParticleObjects.push_back(gameObject);
		}
	}
}

void Camera::SortShadowObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	mShadowObjects.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		if (gameObject->IsStatic())
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum()) {
			BoundingOrientedBox boundingBox = gameObject->GetMeshRenderer()->GetBoundingBox();
			boundingBox.Transform(boundingBox, gameObject->GetTransform()->GetLocalToWorldMatrix());
			if (!IsInFrustum(boundingBox))
				continue;
		}

		mShadowObjects.push_back(gameObject);
	}
}

void Camera::Render_Deferred()
{
	sMatView = mMatView;
	sMatProjection = mMatProjection;

	for (auto& gameObject : mDeferredObjects)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}

void Camera::Render_Forward()
{
	sMatView = mMatView;
	sMatProjection = mMatProjection;

	for (auto& gameObject : mForwardObjects)
	{
		gameObject->GetMeshRenderer()->Render();
	}

	for (auto& gameObject : mParticleObjects)
	{
		gameObject->GetParticleSystem()->Render();
	}
}

void Camera::Render_Shadow()
{
	sMatView = mMatView;
	sMatProjection = mMatProjection;

	for (auto& gameObject : mShadowObjects)
	{
		gameObject->GetMeshRenderer()->RenderShadow();
	}
}
