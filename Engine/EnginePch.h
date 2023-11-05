#pragma once

#include "d3dUtil.h"

// °¢Á¾ typedef
using int8			= __int8;
using int16			= __int16;
using int32			= __int32;
using int64			= __int64;
using uint8			= unsigned __int8;
using uint16		= unsigned __int16;
using uint32		= unsigned __int32;
using uint64		= unsigned __int64;
using Vec2			= DirectX::SimpleMath::Vector2;
using Vec3			= DirectX::SimpleMath::Vector3;
using Vec4			= DirectX::SimpleMath::Vector4;
using Matrix		= DirectX::SimpleMath::Matrix;
using Quaternion	= DirectX::SimpleMath::Quaternion;

template<class T>
using sptr = std::shared_ptr<T>;
template<class T>
using uptr = std::unique_ptr<T>;
template<class T>
using wptr = std::weak_ptr<T>;

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0,
	t1,
	t2,
	t3,
	t4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
};

enum
{
	STATIC_SAMPLER_COUNT = 6,
};

enum
{
	FRAME_RESOURCE_COUNT = 3
};

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	G_BUFFER, // Position, Normal, DIFFUSEALBEDO, SHINESS, FRESNELR0
	LIGHTING, // DIFFUSE, SPECULAR

	END,
};

enum class G_BUFFER_INDEX : uint8
{
	G_POSITION,
	G_NORMAL,
	G_DIFFUSEALBEDO,
	G_FRESNELR0,
	G_SHININESS,

	END,
};

enum class LIGHTING_INDEX : uint8
{
	DIFFUSE = static_cast<uint8>(G_BUFFER_INDEX::END),
	SPECULAR,
	
	END,
};

enum class TEXTURE2D_INDEX : uint8
{
	B_NEWJEANS = static_cast<uint8>(LIGHTING_INDEX::END),
	B_NEWJEANS2,
	B_NEWJEANS3,

	B_LEATHER,
	N_LEATHER,
	R_LEATHER,

	B_WALL,
	N_WALL,
	R_WALL,

	B_PARTICLE,

	END,
};

enum class TEXTURECUBE_INDEX : uint8
{
	SKYBOX = static_cast<uint8>(TEXTURE2D_INDEX::END),

	END,
};

enum class COMPUTE_TEXTURE2D_INDEX : uint8 // uav + srv
{
	TEMP1, 

	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_COUNT = static_cast<uint8>(G_BUFFER_INDEX::END),
	RENDER_TARGET_LIGHTING_COUNT = static_cast<uint8>(LIGHTING_INDEX::END) - RENDER_TARGET_G_BUFFER_GROUP_COUNT,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END),

	COMPUTE_TEXTURE_COUNT = static_cast<uint8>(COMPUTE_TEXTURE2D_INDEX::END),

	TEXTURE_COUNT = static_cast<uint8>(TEXTURECUBE_INDEX::END),
	TEXTURE2D_COUNT = static_cast<uint8>(TEXTURE2D_INDEX::END),
	TEXTURECUBE_COUNT = TEXTURE_COUNT - TEXTURE2D_COUNT,
	TEXTUREFILE_COUNT = TEXTURE_COUNT - static_cast<uint8>(LIGHTING_INDEX::END),
};

struct WindowInfo
{
	HWND	Hwnd; 
	int32	Width; 
	int32	Height;
	bool	Windowed;
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: Pos(p), Uv(u), Normal(n), Tangent(t)
	{
	}

	Vec3 Pos;
	Vec2 Uv;
	Vec3 Normal;
	Vec3 Tangent;
};

struct MinMaxVert
{
	Vec3 Min;
	Vec3 Max;
};

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightInfo
{
	Vec3 Strength = { 0.5f, 0.5f, 0.5f };
	float FallOffStart = 1.0f;                  // point/spot light only
	Vec3 Direction = { 0.0f, -1.0f, 0.0f };		// directional/spot light only
	float FallOffEnd = 10.0f;                   // point/spot light only
	Vec3 Position = { 0.0f, 0.0f, 0.0f };		// point/spot light only
	float SpotPower = 64.0f;                    // spot light only
	int32 LightType;
	Vec3 Padding;
};

#define MAX_LIGHTS	200
struct PassConstants
{
	Matrix		View = Matrix::Identity;
	Matrix		Proj = Matrix::Identity;
	Matrix		ViewProj = Matrix::Identity;
	Vec4		EyePosW = { 0.f, 0.f, 0.f, 0.f };
	float		NearZ = 0.f;
	float		FarZ = 0.f;
	float		Width = 0.f;
	float		Height = 0.f;
	float		TotalTime = 0.f;
	float		DeltaTime = 0.f;
	Vec2		Padding;

	Vec4		AmbientLight = { 0.f, 0.f, 0.f, 1.f };

	uint32		LightCount;
	Vec3		Padding2;
	LightInfo	Lights[MAX_LIGHTS];

	Vec3		CameraRight = { 0.f, 0.f, 0.f };
	float		Padding3;
};

struct ObjectData
{
	Matrix	MatWorld = Matrix::Identity;
	Matrix  MatViewProj = Matrix::Identity;
	Matrix  TexTransform = Matrix::Identity;
	uint32	MaterialIndex = 0;
	Vec3	Padding;
};

struct MaterialData
{
	Vec4	DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vec3	FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float	Roughness = 0.25f;
	Matrix	MatTransform = MathHelper::Identity4x4();
	int32	TextureMapIndex = -1;
	int32	NormalMapIndex = -1;
	int32	RoughnessMapIndex = -1;
	int32	LightIndex = -1;
};

struct ParticleSystemData
{
	float DeltaTime = 0.f;
	float AccTime = 0.f;
	int32 MaxCount = 0;
	int32 AddCount = 0;
	float MinLifeTime = 0.f;
	float MaxLifeTime = 0.f;
	float MinSpeed = 0.f;
	float MaxSpeed = 0.f;
};

struct ParticleData
{
	Vec3	WorldPos = { 1.f, 1.f, 1.f };
	float	CurTime = 0.f;
	Vec3	WorldDir = { 1.f, 1.f, 1.f };
	float	LifeTime = 0.f;
	int32	Alive = 0;
	Vec3	Padding;
};

#define DEVICE							gEngine->GetDevice()->GetDevice()

#define DESCHEAP						gEngine->GetTableDescHeap()

#define GRAPHICS_CMD_QUEUE				gEngine->GetGraphicsCmdQueue()->GetCmdQueue()
#define GRAPHICS_CMD_LIST				gEngine->GetGraphicsCmdQueue()->GetCmdList()
#define GRAPHICS_RES_CMD_LIST			gEngine->GetGraphicsCmdQueue()->GetResCmdList()
#define GRAPHICS_CMD_ALLOC				gEngine->GetGraphicsCmdQueue()->GetCmdAlloc()

#define COMPUTE_CMD_QUEUE				gEngine->GetComputeCmdQueue()->GetCmdQueue()
#define COMPUTE_CMD_LIST				gEngine->GetComputeCmdQueue()->GetCmdList()
#define COMPUTE_CMD_ALLOC				gEngine->GetComputeCmdQueue()->GetCmdAlloc()

#define CURR_FRAMERESOURCE				gEngine->GetCurrFrameResource()
#define CURR_OBJECT_CB					gEngine->GetCurrFrameResource()->mObjectCB

#define GRAPHICS_ROOT_SIGNATURE			gEngine->GetRootSignature()->GetGraphicsRootSignature()
#define COMPUTE_ROOT_SIGNATURE			gEngine->GetRootSignature()->GetComputeRootSignature()

#define OBJECT_CB						gEngine->GetCurrFrameResource()->mObjectCB
#define PASS_CB							gEngine->GetCurrFrameResource()->mPassCB
#define MATERIAL_DATA					gEngine->GetCurrFrameResource()->mMatData
#define PARTICLE_SYSTEM_DATA			gEngine->GetCurrFrameResource()->mParticleSystemData

extern unique_ptr<class Engine> gEngine;