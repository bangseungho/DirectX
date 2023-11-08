#pragma once
#include "Component.h"

class Terrain : public Component
{
public:
	Terrain();
	virtual ~Terrain();

	void Init(int32 sizeX, int32 sizeZ);

	virtual void Start() override;

	virtual void FinalUpdate() override;

private:
	int32 _sizeX = 15;
	int32 _sizeZ = 15;
	float _maxTesselation = 4.f;
	int32 _width = 0;
	int32 _height = 0;

	shared_ptr<class Material> _material;
};


