#ifndef _SKY_HLSL_
#define _SKY_HLSL_

#include "Params.hlsl"

struct VS_IN
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : POSITION;
};

float4 PS_Main(VS_OUT pin) : SV_Target
{
	return gCubeMap.Sample(gsamLinearWrap, pin.posL);
}

#endif