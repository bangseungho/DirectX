#pragma once
#include "Component.h"

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;

public:
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightStrenth(const Vec3& strength) { _lightInfo.strength = strength; }
	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<int32>(type); }
	void SetFallOff(float start, float end) { _lightInfo.fallOffStart = start, _lightInfo.fallOffEnd = end; }
	void SetSpotPower(float spotPower) { _lightInfo.spotPower = spotPower; }

private:
	LightInfo _lightInfo = {};
};

