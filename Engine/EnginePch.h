#pragma once

#include "d3dUtil.h"

// 각종 typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= XMFLOAT2;
using Vec3		= XMFLOAT3;
using Vec4		= XMFLOAT4;
using Vec4x4	= XMFLOAT4X4;
using Matrix	= XMMATRIX;

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
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};

enum
{
	STATIC_SAMPLER_COUNT = 6,
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
	Vec3 pos;
	Vec4 color;
	Vec2 uv;
};

struct ObjectConstants
{
	Vec4 offset = Vec4(0.f, 0.f, 0.f, 0.f);
	Vec4 color = Vec4(0.5f, 0.5f, 0.5f, 0.f);
};

struct PassConstants
{
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Proj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 InvViewProj = MathHelper::Identity4x4();
	DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
};

struct MaterialConstants
{
	void SetDiffuse(Vec4 diffuse) { DiffuseAlbedo = diffuse; }
	void SetFresnel(Vec3 fresnel) { FresnelR0 = fresnel; }
	void SetRoughness(float roughness) { Roughness = roughness; }
	void SetMatTransform(Vec4x4 matTransform) { MatTransform = matTransform; }
	void SetTexOn(bool texOn) { TexOn = texOn; }

	Vec4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vec3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;
	Vec4x4 MatTransform = MathHelper::Identity4x4();
	bool TexOn = false;
};

#define DEVICE					gEngine->GetDevice()->GetDevice()

#define CMD_QUEUE				gEngine->GetCmdQueue()->GetCmdQueue()
#define CMD_LIST				gEngine->GetCmdQueue()->GetCmdList()
#define CMD_ALLOC				gEngine->GetCmdQueue()->GetCmdAlloc()

#define CURR_OBJECT_CB			gEngine->GetCmdQueue()->GetCurrFrameResource()->ObjectCB
#define ROOT_SIGNATURE			gEngine->GetRootSignature()->GetSignature()

#define CB(type)				gEngine->GetCmdQueue()->GetCurrFrameResource()->GetConstantBuffer(type)

extern unique_ptr<class Engine> gEngine;