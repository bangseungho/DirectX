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
	const LightInfo& GetLightInfo() { return mLightInfo; }

	void SetLightStrenth(const Vec3& Strength) { mLightInfo.Strength = Strength; }
	void SetLightDirection(const Vec3& Direction) { mLightInfo.Direction = Direction; }

	void SetLightType(LIGHT_TYPE type);
	void SetFallOff(float start, float end) { mLightInfo.FallOffStart = start, mLightInfo.FallOffEnd = end; }
	void SetSpotPower(float SpotPower) { mLightInfo.SpotPower = SpotPower; }

	void SetLightIndex(int8 index) { mLightIndex = index; }

private:
	LightInfo					mLightInfo = {};
	int8						mLightIndex = -1;
	shared_ptr<class Mesh>		mVolumeMesh;
	shared_ptr<class Material>	mLightMaterial;
};

