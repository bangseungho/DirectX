#include "pch.h"
#include "Terrain.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Texture.h"
#include "Engine.h"

Terrain::Terrain() : Component(COMPONENT_TYPE::TERRAIN)
{

}

Terrain::~Terrain()
{
}

void Terrain::Init(int32 sizeX, int32 sizeZ)
{
	_sizeX = sizeX;
	_sizeZ = sizeZ;
}

void Terrain::Start()
{

}

void Terrain::FinalUpdate()
{
	_material = GET_SINGLE(Resources)->Get<Material>("Terrain");

	TerrainData terrainData;
	terrainData.SizeX = _sizeX;
	terrainData.SizeZ = _sizeZ;
	terrainData.MaxTessellationLevel = _maxTesselation;

	sptr<Texture> heigtMap = GET_SINGLE(Resources)->Get<Texture>("Snow_Height");
	terrainData.Resolution = Vec2(heigtMap->GetWidth(), heigtMap->GetHeight());
	terrainData.MinMaxDistance = Vec2(1000.f, 5000.f);

	TERRAIN_DATA->CopyData(0, terrainData);

	shared_ptr<Camera> mainCamera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
	if (mainCamera == nullptr)
		return;

	Vec3 pos = mainCamera->GetTransform()->GetLocalPosition();
}