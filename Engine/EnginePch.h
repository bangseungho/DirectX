#pragma once

#include "d3dUtil.h"

// 각종 typedef
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
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END),
	REGISTER_COUNT = SRV_REGISTER_COUNT,
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
	G_BUFFER, // POSITION, NORMAL, DIFFUSEALBEDO, SHINESS, FRESNELR0
	END,
};

enum class SWAP_CHAIN_BUFFER_INDEX : uint8
{
	CURR_BACK_BUFFER,
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

enum class TEXTURE2D_INDEX : uint8
{
	B_NEWJEANS = static_cast<uint8>(G_BUFFER_INDEX::END),
	B_NEWJEANS2,
	B_NEWJEANS3,

	B_LEATHER,
	N_LEATHER,
	R_LEATHER,

	B_WALL,
	N_WALL,
	R_WALL,

	END,
};

enum class TEXTURECUBE_INDEX : uint8
{
	SKYBOX = static_cast<uint8>(TEXTURE2D_INDEX::END),
	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_COUNT = static_cast<uint8>(G_BUFFER_INDEX::END),
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END),

	TEXTURE_BACKBUFFER_COUNT = static_cast<uint8>(SWAP_CHAIN_BUFFER_INDEX::END),
	TEXTURE_COUNT = static_cast<uint8>(TEXTURECUBE_INDEX::END) + TEXTURE_BACKBUFFER_COUNT,
	TEXTURE2D_COUNT = static_cast<uint8>(TEXTURE2D_INDEX::END) + TEXTURE_BACKBUFFER_COUNT,
	TEXTURECUBE_COUNT = TEXTURE_COUNT - TEXTURE2D_COUNT,
	TEXTUREFILE_COUNT = TEXTURE_COUNT - RENDER_TARGET_G_BUFFER_GROUP_COUNT - TEXTURE_BACKBUFFER_COUNT,
};

struct WindowInfo
{
	HWND	hwnd; // 출력 윈도우
	int32	width; // 너비
	int32	height; // 높이
	bool	windowed; // 창모드 or 전체화면
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
};

struct MinMaxVert
{
	Vec3 min;
	Vec3 max;
};

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightInfo
{
	Vec3 strength = { 0.5f, 0.5f, 0.5f };
	float fallOffStart = 1.0f;                  // point/spot light only
	Vec3 direction = { 0.0f, -1.0f, 0.0f };		// directional/spot light only
	float fallOffEnd = 10.0f;                   // point/spot light only
	Vec3 position = { 0.0f, 0.0f, 0.0f };		// point/spot light only
	float spotPower = 64.0f;                    // spot light only
	int32 lightType;
	Vec3 padding;
};

#define MAX_LIGHTS	200
struct PassConstants
{
	Matrix view = Matrix::Identity;
	Matrix proj = Matrix::Identity;
	Matrix viewProj = Matrix::Identity;
	Vec4 eyePosW = { 0.f, 0.f, 0.f, 0.f };
	float nearZ = 0.f;
	float farZ = 0.f;
	float totalTime = 0.f;
	float deltaTime = 0.f;

	Vec4 ambientLight = { 0.f, 0.f, 0.f, 1.f };

	uint32		lightCount;
	Vec3		padding;
	LightInfo	lights[MAX_LIGHTS];
};

struct ObjectConstants
{
	Matrix	matWorld = Matrix::Identity;
	Matrix  matViewProj = Matrix::Identity;
	Matrix  texTransform = Matrix::Identity;
	uint32	materialIndex = 0;
	uint32  isUI = 0;
	Vec2	padding;
};

struct MaterialConstants
{
	Vec4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vec3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;
	Matrix MatTransform = MathHelper::Identity4x4();
	uint32 TextureMapIndex = -1;
	uint32 NormalMapIndex = -1;
	uint32 RoughnessMapIndex = -1;
	uint32 padding;
};

#define DEVICE					gEngine->GetDevice()->GetDevice()

#define DESCHEAP				gEngine->GetTableDescHeap()

#define CMD_QUEUE				gEngine->GetCmdQueue()->GetCmdQueue()
#define CMD_LIST				gEngine->GetCmdQueue()->GetCmdList()
#define CMD_ALLOC				gEngine->GetCmdQueue()->GetCmdAlloc()

#define CURR_FRAMERESOURCE		gEngine->GetCurrFrameResource()
#define CURR_OBJECT_CB			gEngine->GetCurrFrameResource()->ObjectCB
#define ROOT_SIGNATURE			gEngine->GetRootSignature()->GetSignature()

#define OBJECT_CB gEngine->GetCurrFrameResource()->ObjectCB
#define PASS_CB gEngine->GetCurrFrameResource()->PassCB
#define MATERIAL_CB gEngine->GetCurrFrameResource()->MaterialCB

extern unique_ptr<class Engine> gEngine;