#pragma once
#include "Component.h"

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;
	void Render();

public:
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightStrenth(const Vec3& strength) { _lightInfo.strength = strength; }
	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetLightType(LIGHT_TYPE type);
	void SetFallOff(float start, float end) { _lightInfo.fallOffStart = start, _lightInfo.fallOffEnd = end; }
	void SetSpotPower(float spotPower) { _lightInfo.spotPower = spotPower; }

	void SetLightIndex(int8 index) { _lightIndex = index; }

private:
	LightInfo _lightInfo = {};

	int8 _lightIndex = -1;
	shared_ptr<class Mesh> _volumeMesh;
	shared_ptr<class Material> _lightMaterial;
};

