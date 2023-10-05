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
using Matrix	= XMMATRIX;

template<class T>
using sptr = std::shared_ptr<T>;
template<class T>
using uptr = std::unique_ptr<T>;
template<class T>
using wptr = std::weak_ptr<T>;

enum class CBV_REGISTER
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	REGISTER_COUNT = CBV_REGISTER::END,
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
};

struct Transform
{
	Vec4 offset;
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

#define DEVICE					GEngine->GetDevice()->GetDevice()

#define CMD_QUEUE				GEngine->GetCmdQueue()->GetCmdQueue()
#define CMD_LIST				GEngine->GetCmdQueue()->GetCmdList()
#define CMD_ALLOC				GEngine->GetCmdQueue()->GetCmdAlloc()

#define CURR_OBJECT_CB			GEngine->GetCmdQueue()->GetCurrFrameResource()->ObjectCB
#define ROOT_SIGNATURE			GEngine->GetRootSignature()->GetSignature()

extern unique_ptr<class Engine> GEngine;